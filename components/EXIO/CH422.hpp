/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Declaration for the CH422 driver.
*/

#pragma once

#include "ExioInterface.hpp"
#include "I2CBusInterface.hpp"

class CH422 : public ExioInterface
{
public:
    /**
     * @brief Contructor for the CH422 device
     */
    CH422(I2CBusInterface& i2cBus);

    /**
     * @brief Default destructor.
     */
    ~CH422() override = default;

    /** @copydoc ExioInterface::setHigh */
    EStatus setHigh(EEXIO pin) override;
    /** @copydoc ExioInterface::setLow */
    EStatus setLow(EEXIO pin) override;

    /** @brief I2C address for writing the sys command register9 of this device */
    static constexpr uint8_t smSysCmdAddr{0x24U};
    /** @brief I2C address for writing the output value to a specific pin */
    static constexpr uint8_t smWriteAddr{0x38U};
    /** @brief I2C address for reading the output values of all pins */
    static constexpr uint8_t smReadAddr{0x26U};

private:
    /**
     * @brief Set the mode of the device
     */
    EStatus setMode();

    /**
     * @brief Read the value each pin is outputting
     * 
     * @param pinOutputs - Value being output by the pins
     */
    EStatus readPinOutputs(uint8_t& pinOutputs);

    /** @brief I2C bus interface to perform read and write operations */
    I2CBusInterface& mI2CBus;

    /** @brief SysCommand handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mSysCmdHandle;
    /** @brief Write handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mWriteHandle;
    /** @brief Read handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mReadHandle;

    /** @brief Configuration info for the SysCommand add */
    static constexpr i2c_device_config_t mSysCmdConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smSysCmdAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use default wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };

    /** @brief Configuration info for the Write add */
    static constexpr i2c_device_config_t mWriteConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smWriteAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use default wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };

    /** @brief Configuration info for the Read add */
    static constexpr i2c_device_config_t mReadConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smReadAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use default wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };

    CH422(const CH422&) = delete;                 //! Delete copy-assignment constructors explicitly
    CH422& operator=(const CH422&) = delete;      //! Delete copy constructors explicitly
    CH422(const CH422&&) = delete;                //! Delete move-assignment constructors explicitly
    CH422&& operator=(const CH422&&) = delete;    //! Delete move  constructors explicitly
};
