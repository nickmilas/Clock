/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the class representing the DS3231 rtc chip.
*/

#include "DS3231.hpp"
#include <array>

DS3231::DS3231(I2CBusInterface& i2cBus) :
    RtcHwInterface(),
    mI2CBus{i2cBus}
{
    Status_t status{mI2CBus.addDevice(&mDeviceConfig, mDeviceHandle)};
    assert(status == Status_t::Success);
    printf("Successfully added device with address: 0x%x\n", smDeviceAddr);
}

Status_t DS3231::getTime(rtc_time_t& tm)
{
    //! Before we send the read command, we need to write the address of the first register to read from
    uint8_t readRegister{smClockStartAddr};
    std::array<uint8_t, 7U> rawData;

    Status_t status{mI2CBus.write_read(smDeviceAddr, &readRegister, sizeof(uint8_t), rawData.data(), rawData.size())};
    if (status == Status_t::Success)
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

Status_t DS3231::setTime(const rtc_time_t& tm)
{
    //! First byte of data is the starting address, other seven bytes should contain time info
    uint8_t standardTimeBit;
    uint8_t afternoonBit;
    std::array<uint8_t, 8U> rawData = {0U};
    uint8_t centuryBit{(isCenturyBitOn()) ? static_cast<uint8_t>(1U << 7U) : static_cast<uint8_t>(0U)};

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
