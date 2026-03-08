/*
* Copyright 2026 - Nicholas Milas Clk project
* Definition of the class to wrap the esp-idf i2c_master class.
*/

#include "I2CBusMaster.hpp"

I2CBusMaster::I2CBusMaster() :
    I2CBusInterface{}
{
    esp_err_t err{i2c_new_master_bus(&mBusConfig, &mHandle)};
    assert(err == ESP_OK); // Make sure we created this bus properly, otherwise blow shit up !!!
    printf("Successfully created i2c bus %d", mBusConfig.i2c_port);
}

Status_t I2CBusMaster::addDevice(const i2c_device_config_t* config)
{
    Status_t status{Status_t::Success};
    do
    {
        if (config == nullptr)
        {
            status = Status_t::Memory;
            break;
        }

        i2c_master_dev_handle_t newDevice;
        esp_err_t err{i2c_master_bus_add_device(mHandle, config, &newDevice)};
        if (err != ESP_OK)
        {
            // Make sure this device was added properly to the bus
            status = Status_t::Error;
            break;
        }

        if (mDeviceToHandle.insert({config->device_address, newDevice}).second)
        {
            // Duplicate device?
            status = Status_t::Error;
            break;
        }
    } while(0U);
    return status;
}

Status_t I2CBusMaster::write(uint16_t address, uint8_t* pData, size_t length)
{
    Status_t status{Status_t::Error};

    auto iter{mDeviceToHandle.find(address)};
    if (iter != mDeviceToHandle.end())
    {
        if (pData != nullptr)
        {
            i2c_master_dev_handle_t& dev{iter->second};
            esp_err_t err{i2c_master_transmit(dev, pData, length, -1)}; // -1 means to wait forever for this transmition
            if (err == ESP_OK)
            {
                // Woohoo!
                status = Status_t::Success;
            }
        }
    }

    return status;
}

Status_t I2CBusMaster::read(uint16_t address, uint8_t* pData, size_t length)
{
    Status_t status{Status_t::Error};

    auto iter{mDeviceToHandle.find(address)};
    if (iter != mDeviceToHandle.end())
    {
        if (pData != nullptr)
        {
            i2c_master_dev_handle_t& dev{iter->second};
            esp_err_t err{i2c_master_receive(dev, pData, length, -1)}; // -1 means to wait forever for this transmition
            if (err == ESP_OK)
            {
                // Skadoosh
                status = Status_t::Success;
            }
        }
    }

    return status;
}
