/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Class to represent the DS3231 rtc chip.
*/

#pragma once

#include "RtcHwInterface.hpp"
#include "I2CBusInterface.hpp"

class DS3231 : public RtcHwInterface
{
public:
    /** @brief I2C address of this device */
    static constexpr uint8_t smDeviceAddr{0x68U};
    /** @brief Starting address for the clock/calender info */
    static constexpr uint8_t smClockStartAddr{0x00U};
    /** @brief Starting address for alarm 1 */
    static constexpr uint8_t smAlarmOneStartAddr{0x07U};
    /** @brief Starting address for alarm 2 */
    static constexpr uint8_t smAlarmTwoStartAddr{0x0BU};

    /**
     * @brief Constructor for a DS3231 chip
     * 
     * @param i2cBus - I2C bus that the device will use to fetch and set time
     */
    DS3231(I2CBusInterface& i2cBus);

    /**
     * @brief Default destructor
     */
    ~DS3231() override = default;

    /** @copydoc RtcHwInterface::getTime */
    Status_t getTime(time_t& tm);
    /** @copydoc RtcHwInterface::setTime */
    Status_t setTime(const time_t& tm);

private:
    /**
     * @brief Helper function to convert from decimal to binary-coded decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in BCD
     */
    uint8_t decimalToBCD(const uint8_t& val);

    /**
     * @brief Helper function to convert from binary-coded decimal to decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in Decimal
     */
    uint8_t BCDToDecimal(const uint8_t& val);

    /** @brief I2C bus interface to perform read and write operations */
    I2CBusInterface& mI2CBus;

    static constexpr i2c_device_config_t mDeviceConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smDeviceAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use defualt wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };
};
