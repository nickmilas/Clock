/*
* Copyright 2026 - Nicholas Milas Clk project
* @brief Interface class that can be extended to wrap either an i2c_master or i2c_slave.
*/

#pragma once

extern "C"
{
    #include "driver/i2c_master.h"
}

#include "Status.hpp"

class I2CBusInterface
{
public:
    /**
     * @brief Default constructor
     */
    I2CBusInterface() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~I2CBusInterface() = default;

    /**
     * @brief Wrapper to add a new device to a given I2C bus instance
     * 
     * @param config - Configuration info for the device to be added to the bus
     */
    virtual EStatus addDevice(const i2c_device_config_t* config, i2c_master_dev_handle_t newDevice) = 0;

    /**
     * @brief Wrapper to perform an I2C write operation
     * 
     * @param address - Address of the device to write to
     * @param pData - Data to be sent on the bus
     * @param length - How many bytes of data to send
     */
    virtual EStatus write(uint16_t address, uint8_t* pData, size_t length) = 0;

    /**
     * @brief Wrapper to perform an I2C read operation
     * 
     * @param address - Address of the device to read from
     * @param pData - Data to be sent on the bus
     * @param length - How many bytes of data to send
     */
    virtual EStatus read(uint16_t address, uint8_t* pData, size_t length) = 0;

    /**
     * @brief Wrapper to perform an I2C write/read operation - Needed to perform multi-register reads
     * 
     * @param address - Address of the device to write/read to
     * @param writeBuff - Data to be written
     * @param writeLength - Amount of data to be written
     * @param readBuff - Data to be read
     * @param readLength - Amount of data to be read
     */
    virtual EStatus write_read(uint16_t address, uint8_t* writeBuff, size_t writeLength, uint8_t* readBuff, size_t readLength) = 0;

private:
    I2CBusInterface(const I2CBusInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    I2CBusInterface& operator=(const I2CBusInterface&) = delete;      //! Delete copy constructors explicitly
    I2CBusInterface(const I2CBusInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    I2CBusInterface&& operator=(const I2CBusInterface&&) = delete;    //! Delete move  constructors explicitly
};
