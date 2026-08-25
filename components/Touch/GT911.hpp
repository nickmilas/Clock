/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Declaration for the GT911 driver.
*/

#pragma once

extern "C"
{
    #include "driver/gpio.h"
    #include "esp_attr.h"
}

#include "I2CBusInterface.hpp"
#include "TouchHwInterface.hpp"

class GT911 : public TouchHwInterface
{
public:
    /**
     * @brief Constructor for the GT911 driver
     * 
     * @param i2cBus - I2C bus that the device will use to fetch and set time
     */
    GT911(I2CBusInterface& i2cBus);

    /**
     * @brief Default destructor.
     */
    ~GT911() = default;

    /** @brief I2C address of this device */
    static constexpr uint8_t smDeviceAddr{0x5DU};

    /** @copydoc TouchHwInterface::readTouch */
    EStatus readTouch(uint16_t& x, uint16_t& y) override;

private:
    /**
     * @brief Helper function to fire off our alarm expired event
     *  -> Marking function as static so it does not belong to a GT911 object and can match the void (*)(void* arg) signature
     * 
     * @param pArgs - Self pass
     */
    static void IRAM_ATTR touchInterruptHandler(void* pArgs);

    /**
     * @brief Helper function to intiate the startup sequence for the gt911 to start sending commands
     */
    EStatus startupDevice();

    /** @brief Reference to the I2C to be used to communicate with the device */
    I2CBusInterface& mI2CBus;

    /** @brief Device handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mDeviceHandle;

    /** @brief Configuration info for the GT911 */
    static constexpr i2c_device_config_t mDeviceConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smDeviceAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use default wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };

    /** @brief Gpio pinout number from where the interrupt will arrive from (Touch interrupt pin) */
    static constexpr gpio_num_t smInteruptPin{GPIO_NUM_4};

    /** @brief Gpio configuration for receiving the interrupt from the touch controller */
    static constexpr gpio_config_t smInteruptPinConfig = {
        .pin_bit_mask = static_cast<uint64_t>(1ULL << smInteruptPin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };

    GT911(const GT911&) = delete;                 //! Delete copy-assignment constructors explicitly
    GT911& operator=(const GT911&) = delete;      //! Delete copy constructors explicitly
    GT911(const GT911&&) = delete;                //! Delete move-assignment constructors explicitly
    GT911&& operator=(const GT911&&) = delete;    //! Delete move  constructors explicitly
};
