/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the class representing the DS3231 rtc chip.
*/

#include "DS3231.hpp"
#include <array>

DS3231::DS3231(I2CBusInterface& i2cBus) :
    mI2CBus{i2cBus}
{
    Status_t status{mI2CBus.addDevice(&mDeviceConfig)};
    assert(status != Status_t::Success);
    printf("Successfully added device with address: %d", smDeviceAddr);
}

Status_t DS3231::getTime(time_t& tm)
{
    /* First byte of data is the starting address, other seven bytes should contain time info */
    std::array<uint8_t, 8U> rawData = {0U};
    rawData[0U] = smClockStartAddr;

    Status_t status{mI2CBus.read(smClockStartAddr, rawData.data(), rawData.size())};
    if (status == Status_t::Success)
    {
        /* Extract data */
        tm.sec = BCDToDecimal(rawData[1U]);
        tm.min = BCDToDecimal(rawData[2U]);
        tm.hour = BCDToDecimal(rawData[3U]);
        tm.day = BCDToDecimal(rawData[4U]);
        tm.date = BCDToDecimal(rawData[5U]);
        tm.month = BCDToDecimal(rawData[6U]);
        tm.year = BCDToDecimal(rawData[7U]);
    }

    return status;
}

Status_t DS3231::setTime(const time_t& tm)
{
    /* First byte of data is the starting address, other seven bytes should contain time info */
    std::array<uint8_t, 8U> rawData = {0U};
    rawData[0U] = smClockStartAddr;

    /* Convert all our info into BCD so the rtc can handle it properly */
    rawData[1U] = decimalToBCD(tm.sec);
    rawData[2U] = decimalToBCD(tm.min);
    rawData[3U] = decimalToBCD(tm.hour);
    rawData[4U] = decimalToBCD(tm.day);
    rawData[5U] = decimalToBCD(tm.date);
    rawData[6U] = decimalToBCD(tm.month);
    rawData[7U] = decimalToBCD(tm.year);

    Status_t status{mI2CBus.write(smClockStartAddr, rawData.data(), rawData.size())};

    return status;
}

uint8_t DS3231::decimalToBCD(const uint8_t& val)
{
    /* Convert our number to BCD - The largest value possible is 9 per 4-bit segment */
    uint8_t result{0U};
    uint8_t lowerBits{static_cast<uint8_t>(val % 10U)};
    uint8_t upperBits{static_cast<uint8_t>(val / 10U)};

    /* Cap our upper bits since they can overflow */
    if (upperBits > 9U)
    {
        upperBits = 9U;
    }
    upperBits <<= 4U;

    result |= lowerBits;
    result |= upperBits;

    return result;
}

uint8_t DS3231::BCDToDecimal(const uint8_t& val)
{
    uint8_t result{0U};
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

    result = (upperBits * 10U) + lowerBits;

    return result;
}
