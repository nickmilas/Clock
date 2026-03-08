/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the class representing the DS3231 rtc chip.
*/

#include "DS3231.hpp"

DS3231::DS3231(I2CBusInterface& i2cBus) :
    mI2CBus{i2cBus}
{
    Status_t status{mI2CBus.addDevice(&mDeviceConfig)};
    assert(status != Status_t::Success);
    printf("Successfully added device with address: %d", mDeviceConfig.device_address);
}

Status_t DS3231::getTime(time_t& tm)
{
    // TODO perform a read and convert all info info decimal
    return Status_t::Error; //! STUB
}

Status_t DS3231::setTime(const time_t& tm)
{
    // TODO perform a write and convert all info to BCD
    return Status_t::Error; //! STUB
}

uint8_t DS3231::decimalToBCD(const uint8_t& val)
{
    uint8_t result{0U};

    /* Convert our number to BCD - The largest a value can be is 9 per 4-bit segment */
    uint8_t lowerBits{static_cast<uint8_t>(val % 10U)};
    uint8_t upperBits{static_cast<uint8_t>(val / 10U)};

    /* Cap our upper bits since they have potential for flowing over */
    if (upperBits > 9)
    {
        upperBits = 9;
    }
    upperBits <<= 4;

    result |= lowerBits;
    result |= upperBits;

    return result;
}

uint8_t DS3231::BCDToDecimal(const uint8_t& val)
{
    // TODO convert from BCD to decimal here (use bitshift stuff)
    return Status_t::Error; //! STUB
}
