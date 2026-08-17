/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the class representing the DS3231 rtc chip.
*/

#include "DS3231.hpp"
#include "I2CBusMaster.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cassert>

DS3231::DS3231(I2CBusInterface& i2cBus) :
    RtcHwInterface(),
    mI2CBus{i2cBus}
{
    esp_err_t err{gpio_config(&smInteruptPinConfig)};
    err |= gpio_install_isr_service(0);
    err |= gpio_isr_handler_add(smInteruptPin, alarmExpirationHandler, static_cast<void*>(this));
    assert(err == ESP_OK);
    printf("RTC: Successfully configured gpio %d for rtc interupt handling\n", static_cast<uint8_t>(smInteruptPin));

    assert(mI2CBus.addDevice(&mDeviceConfig, mDeviceHandle) == EStatus::Success);
    printf("RTC: Successfully added device with address: 0x%x\n", smDeviceAddr);

    bool isTimer;
    if (clearExpiredFlags(isTimer) != EStatus::Success)
    {
        printf("RTC: Failed to clear expired flags on startup!!!\n");
    }

    assert(xTaskCreate(DS3231::taskFunction, "RTC", 4096U, static_cast<void*>(this), (tskIDLE_PRIORITY + 1), nullptr) == pdPASS);
}

EStatus DS3231::getTime(clock::rtc_time_t& tm)
{
    //! Before we send the read command, we need to write the address of the first register to read from
    uint8_t readRegister{smClockStartAddr};
    std::array<uint8_t, 7U> rawData;

    EStatus status{mI2CBus.write_read(smDeviceAddr, &readRegister, sizeof(uint8_t), rawData.data(), rawData.size())};
    if (status == EStatus::Success)
    {
        /* Check for century + am/pm + 12/24 bit changes */
        uint8_t afternoonBitMask{(1U << 5U)};
        uint8_t standardBitMask{(1U << 6U)};
        uint8_t centuryBitMask{(1U << 7U)};

        setStandardTime(rawData[2U] & standardBitMask); // If it is 1, we are using standard time, else we are using military time
        setCenturyBit(rawData[5U] & centuryBitMask); // If it is 1, we rolled over into a new century, otherwise we are still in 20XX

        if (isStandardTime())
        {
            setAfternoon(rawData[2U] & afternoonBitMask); // If it is 1, we are in the afternoon, otherwise it is the morning
            rawData[2U] &= ~afternoonBitMask; // Peel off the !AM/PM bit so we don't throw off our hours
        }

        /* Extract data */
        tm.sec = BCDToDecimal(rawData[0U]);
        tm.min = BCDToDecimal(rawData[1U]);
        tm.hour = BCDToDecimal(rawData[2U] & ~standardBitMask); // Peel off the 12/!24 bit so we don't throw off our hours
        tm.day = BCDToDecimal(rawData[3U]);
        tm.date = BCDToDecimal(rawData[4U]);
        tm.month = BCDToDecimal(rawData[5U] & ~centuryBitMask); // Peel off the century bit so we don't throw off the month
        tm.year = BCDToDecimal(rawData[6U]);
    }

    return status;
}

EStatus DS3231::setTime(const clock::rtc_time_t& tm)
{
    //! First byte of data is the starting address, other seven bytes should contain time info
    uint8_t standardTimeBit{0U};
    uint8_t afternoonBit{0U};
    uint8_t centuryBit{(isCenturyBitOn()) ? static_cast<uint8_t>(1U << 7U) : static_cast<uint8_t>(0U)};
    std::array<uint8_t, 8U> rawData = {0U};

    if (isStandardTime())
    {
        standardTimeBit = static_cast<uint8_t>(1U << 6U);
        if (tm.hour > 12U)
        {
            setAfternoon(true); // Set to the afternoon since our time is > 12
        }
        afternoonBit = (isAfternoon()) ? static_cast<uint8_t>(1U << 5U) : static_cast<uint8_t>(0U);
    }

    /* Convert all our info into BCD so the rtc can handle it properly */
    rawData[0U] = smClockStartAddr;
    rawData[1U] = decimalToBCD(tm.sec);
    rawData[2U] = decimalToBCD(tm.min);
    rawData[3U] = decimalToBCD((tm.hour <= 12U) ? tm.hour : (tm.hour % 12U)) | standardTimeBit | afternoonBit;
    rawData[4U] = decimalToBCD(tm.day);
    rawData[5U] = decimalToBCD(tm.date);
    rawData[6U] = decimalToBCD(tm.month) | centuryBit;
    rawData[7U] = decimalToBCD(tm.year);

    return mI2CBus.write(smDeviceAddr, rawData.data(), rawData.size());
}

EStatus DS3231::setAlarm(const clock::rtc_alarm_t& tm, clock::EAlarm alarm)
{
    uint8_t standardTimeBit{isStandardTime() ? static_cast<uint8_t>(1U << 6U) : static_cast<uint8_t>(0U)};
    uint8_t afternoonBit{isAfternoon() ? static_cast<uint8_t>(1U << 5U) : static_cast<uint8_t>(0U)};
    uint8_t expirationBit{static_cast<uint8_t>(1U << 7U)};
    std::vector<uint8_t> rawData;

    // Calculate future time for our timer
    clock::rtc_time_t currTime;
    getTime(currTime);

    uint8_t forwardMin{static_cast<uint8_t>(currTime.min + tm.min)};
    uint8_t forwardHour{static_cast<uint8_t>(currTime.hour + tm.hour)};
    if (forwardMin >= 60U) // Roll-over
    {
        forwardMin %= 60U;
        forwardHour++;
    }
    if (isStandardTime()) // More roll-over
    {
        if (forwardHour == 12U) // Rolled over to either AM or PM
        {
            // If we are not currently in the afternoon, but the next hour is 12, then set the bit and vice versa
            afternoonBit = !(isAfternoon()) ? static_cast<uint8_t>(1U << 5U) : static_cast<uint8_t>(0U);
        }

        if (forwardHour > 12U)
        {
            forwardHour %= 12U;
        }
    }
    else
    {
        if (forwardHour >= 24U)
        {
            forwardHour %= 24U;
        }
    }

    if (alarm == clock::EAlarm::Alarm)
    {
        rawData.push_back(smAlarmTwoStartAddr);
        rawData.push_back(decimalToBCD(tm.min));
        rawData.push_back(decimalToBCD(tm.hour) | standardTimeBit | afternoonBit);
        rawData.push_back(expirationBit); // Need to set the A2M4 bit so the timer will go off when hours/minutes match
    }
    else // We are a timer
    {
        rawData.push_back(smAlarmOneStartAddr);
        rawData.push_back(decimalToBCD(currTime.sec)); // pushback current seconds 
        rawData.push_back(decimalToBCD(forwardMin));
        rawData.push_back(decimalToBCD(forwardHour) | standardTimeBit | afternoonBit);
        rawData.push_back(expirationBit); // Need to set the A1M4 bit so the timer will go off when hours/minutes/seconds match
    }

    /* Let's make sure the alarm is actually on */
    EStatus status{mI2CBus.write(smDeviceAddr, rawData.data(), rawData.size())};
    if (status == EStatus::Success)
    {
        status = enableAlarm(alarm);
    }

    return status;
}

uint8_t DS3231::decimalToBCD(const uint8_t val) const
{
    /* Convert our number to BCD - The largest value possible is 9 per 4-bit segment */
    uint8_t lowerBits{static_cast<uint8_t>(val % 10U)};
    uint8_t upperBits{static_cast<uint8_t>(val / 10U)};

    /* Cap our upper bits since they can overflow */
    if (upperBits > 9U)
    {
        upperBits = 9U;
    }
    upperBits <<= 4U;

    return static_cast<uint8_t>(upperBits | lowerBits);
}

uint8_t DS3231::BCDToDecimal(const uint8_t val) const
{
    uint8_t lowerBits{static_cast<uint8_t>(val & 0xF)};
    uint8_t upperBits{static_cast<uint8_t>((val >> 4U) & 0xF)};

    /* Cap our values to be at most 9. The RTC should never return values greater than that, but to be cautious. */
    if (lowerBits > 9U)
    {
        lowerBits = 9U;
    }
    if (upperBits > 9U)
    {
        upperBits = 9U;
    }

    return static_cast<uint8_t>((upperBits * 10U) + lowerBits);
}

EStatus DS3231::enableAlarm(clock::EAlarm alarm)
{
    /* Let's read the control register and then add in our new alarm bit (don't want to overwrite everything) */
    uint8_t controlRegister{0U};
    uint8_t address{smControlAddr};

    EStatus status{mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &controlRegister, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("RTC: Failed to set an alarm: %d\n", static_cast<uint8_t>(alarm));
        return status;
    }

    controlRegister |= static_cast<uint8_t>(1U << 2U); // INTCN - route alarm interrupts to !INT/SQW pin (not square wave)
    controlRegister |= (alarm == clock::EAlarm::Timer) ? static_cast<uint8_t>(1U) : static_cast<uint8_t>(1U << 1U);
    std::array<uint8_t, 2U> writeData{ smControlAddr, controlRegister };
    return mI2CBus.write(smDeviceAddr, writeData.data(), writeData.size());
}

EStatus DS3231::clearExpiredFlags(bool& isTimerExpired)
{
    uint8_t address{smControlStatusAddr};
    uint8_t statusRegister{0U};

    EStatus status{mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &statusRegister, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("RTC: Failed to clear expired alarm flags\n");
        return status;
    }

    if (statusRegister & static_cast<uint8_t>(1U))
    {
        statusRegister &= ~(0b00000001); // If timer expired clear the flag
    }
    if (statusRegister & static_cast<uint8_t>(1U << 1U))
    {
        isTimerExpired = true;
        statusRegister &= ~(0b00000010); // If alarm expired clear the flag
    }
    std::array<uint8_t, 2U> writeData{ address, statusRegister };
    return mI2CBus.write(smDeviceAddr, writeData.data(), writeData.size());
}

EStatus DS3231::disableAlarm(clock::EAlarm alarmType)
{
    uint8_t address{smControlAddr};
    uint8_t controlRegister{0U};

    EStatus status{mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &controlRegister, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("RTC: Failed to disable a given alarm");
    }

    controlRegister &= (alarmType == clock::EAlarm::Timer) ? ~(0b00000001) : ~(0b00000010);
    std::array<uint8_t, 2U> writeData{ address, controlRegister };
    return mI2CBus.write(smDeviceAddr, writeData.data(), writeData.size());
}

void DS3231::taskFunction(void* pArgs)
{
    if (pArgs == nullptr)
    {
        printf("RTC: Task args invalid...\n");
        return;
    }

    static DS3231* self{static_cast<DS3231*>(pArgs)};
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
        clock::rtc_time_t readTime{};
        EStatus getStatus{self->getTime(readTime)};
        if (getStatus == EStatus::Success)
        {
            printf("RTC: Time read is %d/%d/%d - %d:%d:%d%s\n", readTime.month,
                                                        readTime.date,
                                                        readTime.year,
                                                        readTime.hour,
                                                        readTime.min,
                                                        readTime.sec,
                                                        (self->isAfternoon()) ? "pm" : "am");
            self->dumpRegisters();
        }
        else
        {
            printf("RTC: Failed to get time!\n");
        }
    }
}

void DS3231::Notify(lambda<RtcHwEvent> event)
{
    for (RtcHwEvent* component : mListeners)
    {
        if (component != nullptr)
        {
            event(*component);
        }
    }
}

void IRAM_ATTR DS3231::alarmExpirationHandler(void* pArgs)
{
    if (pArgs == nullptr)
    {
        return;
    }

    DS3231* self{static_cast<DS3231*>(pArgs)};
    self->Notify([](RtcHwEvent& rListener){
        rListener.eventAlarmExpired();
    });
}

void DS3231::dumpRegisters()
{
    uint8_t statusRegister{0U};
    uint8_t controlRegister{0U};

    uint8_t address{smControlStatusAddr};
    mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &statusRegister, sizeof(uint8_t));

    address = smControlAddr;
    mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &controlRegister, sizeof(uint8_t));

    uint8_t buf[4];
    address = smAlarmOneStartAddr;
    mI2CBus.write_read(smDeviceAddr, &address, 1, buf, 4);

    if (isStandardTime())
    {
        buf[2U] &= ~(0b01100000); // Peel off the !AM/PM bit so we don't throw off our hours
    }

    printf("Control registers: %d%d%d%d %d%d%d%d\n", (controlRegister >> 7) & 1, (controlRegister >> 6) & 1, (controlRegister >> 5) & 1, (controlRegister >> 4) & 1,  (controlRegister >> 3) & 1, (controlRegister >> 2) & 1, (controlRegister >> 1) & 1, (controlRegister & 1));
    printf("Control/Status registers: %d%d%d%d %d%d%d%d\n", (statusRegister >> 7) & 1, (statusRegister >> 6) & 1, (statusRegister >> 5) & 1, (statusRegister >> 4) & 1,  (statusRegister >> 3) & 1, (statusRegister >> 2) & 1, (statusRegister >> 1) & 1, (statusRegister & 1));
    printf("Alarm 1: sec: %d, min: %d, hour: %d, expiration bit: %d\n\n", BCDToDecimal(buf[0]), BCDToDecimal(buf[1]), BCDToDecimal(buf[2]), buf[3]);
}
