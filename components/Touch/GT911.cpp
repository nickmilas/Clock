/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Definition for the GT911 driver.
*/

extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}

#include "GT911.hpp"
#include "I2CBusMaster.hpp"

GT911::GT911(I2CBusInterface& i2cBus) :
    TouchHwInterface(),
    mI2CBus{i2cBus}
{
    assert(startupDevice() == EStatus::Success);
    printf("GT911: Successfully started up device!\n");

    esp_err_t err{gpio_config(&smInteruptPinConfig)};
    err |= gpio_isr_handler_add(smInteruptPin, touchInterruptHandler, static_cast<void*>(this));
    assert(err == ESP_OK);
    printf("GT911: Successfully configured gpio %d for rtc interupt handling\n", static_cast<uint8_t>(smInteruptPin));

    assert(mI2CBus.addDevice(&mDeviceConfig, mDeviceHandle) == EStatus::Success);
    printf("GT911: Successfully added device with address: 0x%x\n", smDeviceAddr);
}

EStatus GT911::startupDevice()
{
    EStatus status{EStatus::Success};

    // TODO - Run startup sequence using ch422

    return status;
}

void IRAM_ATTR GT911::touchInterruptHandler(void* pArgs)
{
    if (pArgs == nullptr)
    {
        return;
    }

    GT911* self{static_cast<GT911*>(pArgs)};
    self->Notify([](TouchHwEvent& rTouchHw)
    {
        rTouchHw.eventTouchDetected();
    });
}

EStatus GT911::readTouch(uint16_t& x, uint16_t& y)
{
    // TODO - use i2c to read coordinates for touch
    return EStatus::Success;
}
