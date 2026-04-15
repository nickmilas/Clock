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
    Status_t getTime(rtc_time_t& tm) override;
    /** @copydoc RtcHwInterface::setTime */
    Status_t setTime(const rtc_time_t& tm) override;

    /** @brief Set the clock to read standard time (active high for standard time - active low for military time) */
    void setStandardTime(bool isStandardTime) { mIsStandardTime = isStandardTime; }
    /** @brief Returns the 12/!24 bit status */
    bool isStandardTime() const { return mIsStandardTime; }

    /** @brief Set whethere we are interpretting time as !AM/PM */
    void setMorningOrAfternoon(bool isAfternoon) { mIsAfternoon = isAfternoon; }
    /** @brief Returns the !AM/PM bit status */
    bool isAfternoon() const { return mIsAfternoon; }

    /** @brief Set the clock to set the century bit */
    void setCenturyBit(bool isNewCentury) { mIsCenturyBitOn = isNewCentury; }
    /** @brief Returns the century bit status */
    bool isCenturyBitOn() const { return mIsCenturyBitOn; }

private:
    /**
     * @brief Helper function to convert from decimal to binary-coded decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in BCD
     */
    uint8_t decimalToBCD(const uint8_t val);

    /**
     * @brief Helper function to convert from binary-coded decimal to decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in Decimal
     */
    uint8_t BCDToDecimal(const uint8_t val);

    /** @brief Flag indicating how we should interpret the current hour reading */
    bool mIsStandardTime{false};
    /** @brief Flag indicating whether time is in AM or PM (only matters if we are using standard time - default to morning) */
    bool mIsAfternoon{false};
    /** @brief Flag indicating whether or not the century bit is on (default to 21st century) */
    bool mIsCenturyBitOn{false};
    /** @brief I2C bus interface to perform read and write operations */
    I2CBusInterface& mI2CBus;
    /** @brief Device handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mDeviceHandle;
    /** @brief Configuration info for the DD3231 chip */
    static constexpr i2c_device_config_t mDeviceConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smDeviceAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use defualt wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };
};
