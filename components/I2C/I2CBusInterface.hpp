/*
* Copyright 2026 - Nicholas Milas Clk project
* @brief Interface class that can be extended to wrap either an i2c_master or i2c_slave.
*/

#pragma once

#include "driver/i2c_master.h"
#include "status.hpp"

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
     * @brief Wrapper to perform an I2C write operation
     * 
     * @param pData - Data to be sent on the bus
     * @param length - How many bytes of data to send
     */
    virtual Status_t write(uint16_t address, uint8_t* pData, size_t length) = 0;

    /**
     * @brief Wrapper to perform an I2C read operation
     * 
     * @param pData - Data to be sent on the bus
     * @param length - How many bytes of data to send
     */
    virtual Status_t read(uint16_t address, uint8_t* pData, size_t length) = 0;

private:
    I2CBusInterface(const I2CBusInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    I2CBusInterface& operator=(const I2CBusInterface&) = delete;      //! Delete copy constructors explicitly
    I2CBusInterface(const I2CBusInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    I2CBusInterface&& operator=(const I2CBusInterface&&) = delete;    //! Delete move  constructors explicitly
};