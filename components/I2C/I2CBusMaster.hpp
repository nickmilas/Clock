/*
* Copyright 2026 - Nicholas Milas Clk project
* Declaration of the class to wrap the esp-idf i2c_master class.
*/

#pragma once

#include "I2CBusInterface.hpp"
#include "driver/gpio.h"
#include <unordered_map>

class I2CBusMaster : public I2CBusInterface
{
public:
    /**
     * @brief Constructor
     */
    I2CBusMaster();

    /**
     * @brief Default destructor.
     */
    ~I2CBusMaster() override = default;

    /**
     * @brief Method to add a new device to the I2C bus
     * 
     * @param config - Configuration details of the device to be added to the bus
     */
    EStatus addDevice(const i2c_device_config_t* config, i2c_master_dev_handle_t newDevice);
    /** @copydoc I2CBusInterface::write */
    EStatus write(uint16_t address, uint8_t* pData, size_t length) override;
    /** @copydoc I2CBusInterface::read */
    EStatus read(uint16_t address, uint8_t* pData, size_t length) override;
    /** @copydoc I2CBusInterface::write_read */
    EStatus write_read(uint16_t address, uint8_t* writeBuff, size_t writeLength, uint8_t* readBuff, size_t readLength);

private:
    /** @brief Handle for managing I2C bus actions */
    i2c_master_bus_handle_t mHandle;
    /** @brief Map of devices to their handle */
    std::unordered_map<uint16_t, i2c_master_dev_handle_t> mDeviceToHandle;

    /** @brief Configurations for creating a i2c_master_bus_handle_t */
    static constexpr i2c_master_bus_config_t mBusConfig {
        .i2c_port = i2c_port_t::I2C_NUM_0,      // Using i2c port 0. Hardcoded here for our purposes, but could later be passed to a constructor
        .sda_io_num = gpio_num_t::GPIO_NUM_8,
        .scl_io_num = gpio_num_t::GPIO_NUM_9,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7U,                // This is the typical default
        .intr_priority = 0,                     // Default priority
        .trans_queue_depth = 0U,                // Default depth
        .flags = { .enable_internal_pullup = true, .allow_pd = false},
    };
};
