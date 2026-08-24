/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Class to represent the DS3231 rtc chip.
*/

#pragma once

extern "C"
{
    #include "driver/gpio.h"
    #include "esp_attr.h"
}

#include "RtcHwInterface.hpp"
#include "I2CBusInterface.hpp"
#include <array>
#include <vector>

class DS3231 : public RtcHwInterface
{
public:
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
    EStatus getTime(clock::rtc_time_t& tm) override;
    /** @copydoc RtcHwInterface::setTime */
    EStatus setTime(const clock::rtc_time_t& tm) override;
    /** @copydoc RtcHwInterface::setAlarm */
    EStatus setAlarm(const clock::rtc_alarm_t& tm, clock::EAlarm alarm) override;
    /** @copydoc RtcHwInterface::clearExpiredFlags */
    EStatus clearExpiredFlags(bool& isTimerExpired) override;
    /** @copydoc RtcHwInterface::disableAlarm */
    EStatus disableAlarm(clock::EAlarm alarmType) override;

    /** @brief I2C address of this device */
    static constexpr uint8_t smDeviceAddr{0x68U};
    /** @brief Starting address for the clock/calender info */
    static constexpr uint8_t smClockStartAddr{0x00U};
    /** @brief Starting address for alarm 1 */
    static constexpr uint8_t smAlarmOneStartAddr{0x07U};
    /** @brief Starting address for alarm 2 */
    static constexpr uint8_t smAlarmTwoStartAddr{0x0BU};
    /** @brief Control register address */
    static constexpr uint8_t smControlAddr{0x0EU};
    /** @brief Control/Status register address */
    static constexpr uint8_t smControlStatusAddr{0x0FU};

private:
    /**
     * @brief Helper function to convert from decimal to binary-coded decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in BCD
     */
    uint8_t decimalToBCD(const uint8_t val) const;

    /**
     * @brief Helper function to convert from binary-coded decimal to decimal
     * 
     * @param val - Value to convert
     * @return uint8_t - Value in Decimal
     */
    uint8_t BCDToDecimal(const uint8_t val) const;
    
    /**
     * @brief Helper function to enable the corresponding RTC alarm interrupt
     * 
     * @param alarm - The alarm (1 or 2) we are going to turn on
     * @return EStatus - Whether the writes were successful or not
     */
    EStatus enableAlarm(clock::EAlarm alarm);

    /** @brief Helper function to display register values */
    void dumpRegisters();

    /** @brief Function responsible for running the rtc task
     *  -> Marking function as static so it does not belong to a DS3231 object and can match the void (*)(void* arg) signature
     * 
     * @param pArgs - Self pass
     */
    static void taskFunction(void* pArgs);

    /** @brief Helper function to fire off our alarm expired event
     *  -> Marking function as static so it does not belong to a DS3231 object and can match the void (*)(void* arg) signature
     * 
     * @param pArgs - Self pass
     */
    static void IRAM_ATTR alarmExpirationHandler(void* pArgs);

    /** @brief I2C bus interface to perform read and write operations */
    I2CBusInterface& mI2CBus;

    /** @brief Device handle pointer that needs to persist through runtime */
    i2c_master_dev_handle_t mDeviceHandle;

    /** @brief Configuration info for the DD3231 chip */
    static constexpr i2c_device_config_t mDeviceConfig {
        .dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
        .device_address = smDeviceAddr,
        .scl_speed_hz = 400000U,
        .scl_wait_us = 0U,                  // Use default wait time
        .flags = { .disable_ack_check = 0 } // Enable ack check
    };

    /** @brief Gpio pinout number from where the interrupt will arrive from (sensor-ad pin) */
    static constexpr gpio_num_t smInteruptPin{GPIO_NUM_6};

    /** @brief Gpio configuration for receiving the interrupt from the rtc */
    static constexpr gpio_config_t smInteruptPinConfig = {
        .pin_bit_mask = static_cast<uint64_t>(1ULL << smInteruptPin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
};
