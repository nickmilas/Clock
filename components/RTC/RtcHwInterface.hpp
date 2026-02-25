/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for a RTC component.
*/

#pragma once

#include "status.hpp"

class IRtcHw
{
public:
    /**
     * @brief Structure used to keep track of time/calender data
     */
    struct time_t{
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
        uint8_t day;
        uint8_t month;
        uint8_t year;
    };

    /**
     * @brief Default constructor.
     */
    IRtcHw() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~IRtcHw() = default;

    /**
     * @brief Initialize the rtc module by adding it as a device on the I2C bus
     */
    virtual void rtcHwInit(void) = 0;

    /**
     * @brief Read the time off of an RTC device
     * 
     * @param tm - The time read off of the RTC
     * @return Status_t - Whether or not the read was successful
     */
    virtual Status_t getTime(time_t& tm) = 0;

    /**
     * @brief Set the Time object
     * 
     * @param tm - The time to set on the RTC
     * @return Status_t - Whether or not the write was successful
     */
    virtual Status_t setTime(time_t tm) = 0;

private:
    IRtcHw(const IRtcHw&) = delete;                 //! Delete copy-assignment constructors explicitly
    IRtcHw& operator=(const IRtcHw&) = delete;      //! Delete copy constructors explicitly
    IRtcHw(const IRtcHw&&) = delete;                //! Delete move-assignment constructors explicitly
    IRtcHw&& operator=(const IRtcHw&&) = delete;    //! Delete move  constructors explicitly
};
