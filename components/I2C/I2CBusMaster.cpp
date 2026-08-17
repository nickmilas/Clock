/*
* Copyright 2026 - Nicholas Milas Clk project
* Definition of the class to wrap the esp-idf i2c_master class.
*/

#include "I2CBusMaster.hpp"

I2CBusMaster::I2CBusMaster() :
    I2CBusInterface()
{
    esp_err_t err{i2c_new_master_bus(&mBusConfig, &mHandle)};
    assert(err == ESP_OK); // Make sure we created this bus properly, otherwise blow shit up !!!
    printf("I2C: Successfully created i2c bus 0x%x\n", mBusConfig.i2c_port);
}

EStatus I2CBusMaster::addDevice(const i2c_device_config_t* config, i2c_master_dev_handle_t newDevice)
{
    EStatus status{EStatus::Success};
    do
    {
        if (config == nullptr)
        {
            status = EStatus::Memory;
            break;
        }

        esp_err_t err{i2c_master_bus_add_device(mHandle, config, &newDevice)};
        if (err != ESP_OK)
        {
            // Make sure this device was added properly to the bus
            status = EStatus::Error;
            break;
        }

        if (!mDeviceToHandle.insert({config->device_address, newDevice}).second)
        {
            // Duplicate device?
            status = EStatus::Error;
            break;
        }
    } while(0U);
    return status;
}

EStatus I2CBusMaster::write(uint16_t address, uint8_t* pData, size_t length)
{
    EStatus status{EStatus::Error};

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
                status = EStatus::Success;
            }
            else
            {
                printf("I2C: Transmit error: %s\n", esp_err_to_name(err));
            }
        }
        else
        {
            printf("I2C: Bad data!\n");
        }
    }
    else
    {
        printf("I2C: Could not find device: 0x%x\n", address);
    }

    return status;
}

EStatus I2CBusMaster::read(uint16_t address, uint8_t* pData, size_t length)
{
    EStatus status{EStatus::Error};

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
                status = EStatus::Success;
            }
            else
            {
                printf("I2C: Receive error: %s\n", esp_err_to_name(err));
            }
        }
        else
        {
            printf("I2C: Bad data!\n");
        }
    }
    else
    {
        printf("I2C: Could not find device: 0x%x\n", address);
    }

    return status;
}

EStatus I2CBusMaster::write_read(uint16_t address, uint8_t* writeBuff, size_t writeLength, uint8_t* readBuff, size_t readLength)
{
    EStatus status{EStatus::Error};

    auto iter{mDeviceToHandle.find(address)};
    if (iter != mDeviceToHandle.end())
    {
        if ((writeBuff != nullptr) && (readBuff != nullptr))
        {
            i2c_master_dev_handle_t& dev{iter->second};
            esp_err_t err{i2c_master_transmit_receive(dev, writeBuff, writeLength, readBuff, readLength,-1)}; // -1 means to wait forever for this transmition
            if (err == ESP_OK)
            {
                // Oh yeah
                status = EStatus::Success;
            }
            else
            {
                printf("I2C: Transmit_receive error: %s\n", esp_err_to_name(err));
            }
        }
        else
        {
            printf("I2C: Bad write or read data!\n");
        }
    }
    else
    {
        printf("I2C: Could not find device: 0x%x\n", address);
    }

    return status;
}
