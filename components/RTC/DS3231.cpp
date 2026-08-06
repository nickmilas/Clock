/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the class representing the DS3231 rtc chip.
*/

#include "DS3231.hpp"
#include <vector>

DS3231::DS3231(I2CBusInterface& i2cBus) :
    RtcHwInterface(),
    mI2CBus{i2cBus}
{
    EStatus status{mI2CBus.addDevice(&mDeviceConfig, mDeviceHandle)};
    assert(status == EStatus::Success);
    printf("Successfully added device with address: 0x%x\n", smDeviceAddr);
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
            setMorningOrAfternoon(rawData[2U] & afternoonBitMask); // If it is 1, we are in the afternoon, otherwise it is the morning
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
    uint8_t standardTimeBit;
    uint8_t afternoonBit;
    uint8_t centuryBit{(isCenturyBitOn()) ? static_cast<uint8_t>(1U << 7U) : static_cast<uint8_t>(0U)};
    std::array<uint8_t, 8U> rawData = {0U};

    if (isStandardTime())
    {
        standardTimeBit = static_cast<uint8_t>(1U << 6U);
        afternoonBit = (isAfternoon()) ? static_cast<uint8_t>(1U << 5U) : static_cast<uint8_t>(0U);
    }
    else
    {
        standardTimeBit = 0U;
        afternoonBit = 0U;
    }

    /* Convert all our info into BCD so the rtc can handle it properly */
    rawData[0U] = smClockStartAddr;
    rawData[1U] = decimalToBCD(tm.sec);
    rawData[2U] = decimalToBCD(tm.min);
    rawData[3U] = decimalToBCD(tm.hour) | standardTimeBit | afternoonBit;
    rawData[4U] = decimalToBCD(tm.day);
    rawData[5U] = decimalToBCD(tm.date);
    rawData[6U] = decimalToBCD(tm.month) | centuryBit;
    rawData[7U] = decimalToBCD(tm.year);

    return mI2CBus.write(smDeviceAddr, rawData.data(), rawData.size());
}

EStatus DS3231::setAlarm(const clock::rtc_alarm_t& tm, clock::EAlarm alarm)
{
    uint8_t standardTimeBit;
    uint8_t afternoonBit;
    uint8_t expirationBit{static_cast<uint8_t>(1U << 7U)};
    std::vector<uint8_t> rawData;

    if (isStandardTime())
    {
        standardTimeBit = static_cast<uint8_t>(1U << 6U);
        afternoonBit = (isAfternoon()) ? static_cast<uint8_t>(1U << 5U) : static_cast<uint8_t>(0U);
    }
    else
    {
        standardTimeBit = 0U;
        afternoonBit = 0U;
    }

    if (alarm == clock::EAlarm::Alarm)
    {
        rawData.push_back(smAlarmOneStartAddr);
        rawData.push_back(0U); // If we are an alarm, we always want to match at 0 seconds
        rawData.push_back(decimalToBCD(tm.min));
        rawData.push_back(decimalToBCD(tm.hour) | standardTimeBit | afternoonBit);
        rawData.push_back(expirationBit); // Need to set the A1M4 bit so the timer will go off when hours/minutes/seconds match
    }
    else // We are a timer
    {
        uint8_t shortTimerBit{(tm.isShortTimer) ? static_cast<uint8_t>(1U << 7U) : static_cast<uint8_t>(0U)};
        rawData.push_back(smAlarmTwoStartAddr);
        rawData.push_back(decimalToBCD(tm.min));
        rawData.push_back(decimalToBCD(tm.hour) | standardTimeBit | afternoonBit | shortTimerBit); // If isShortTimer, then go off when minutes match
        rawData.push_back(expirationBit); // Need to set the A1M4 bit so the timer will go off when hours/minutes match
    }

    /* Let's make sure the alarm is actually on */
    EStatus status{setAlarmBit(alarm)};
    if (status == EStatus::Success)
    {
        // Nice.
        status = mI2CBus.write(smDeviceAddr, rawData.data(), rawData.size());
    }

    return status;
}

uint8_t DS3231::decimalToBCD(const uint8_t val)
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

uint8_t DS3231::BCDToDecimal(const uint8_t val)
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

EStatus DS3231::setAlarmBit(clock::EAlarm alarm)
{
    /* Let's read the control register and then add in our new alarm bit (don't want to overwrite everything) */
    uint8_t controlRegister{0U};
    uint8_t address{smControlAddr};

    EStatus status{mI2CBus.write_read(smDeviceAddr, &address, sizeof(uint8_t), &controlRegister, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("Failed to set an alarm: %d", static_cast<uint8_t>(alarm));
        return status;
    }

    controlRegister |= (alarm == clock::EAlarm::Alarm) ? static_cast<uint8_t>(1U) : static_cast<uint8_t>(1U << 1U);
    std::array<uint8_t, 2U> writeData{ smControlAddr, controlRegister };
    return mI2CBus.write(smDeviceAddr, writeData.data(), writeData.size());
}
