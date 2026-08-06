/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for a RTC component.
*/

#pragma once

#include "status.hpp"
#include "clock_enums.hpp"

class RtcHwInterface
{
public:
    /**
     * @brief Default constructor.
     */
    RtcHwInterface() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~RtcHwInterface() = default;

    /**
     * @brief Read the time off of an RTC device
     * 
     * @param tm - The time read off of the RTC
     * @return EStatus - Whether or not the read was successful
     */
    virtual EStatus getTime(clock::rtc_time_t& tm) = 0;

    /**
     * @brief Set the Time object
     * 
     * @param tm - The time to set on the RTC
     * @return EStatus - Whether or not the write was successful
     */
    virtual EStatus setTime(const clock::rtc_time_t& tm) = 0;

    /**
     * @brief Set an alarm based off the Time struct
     * 
     * @param tm - The time used to set the alarm
     * @return EStatus - Whether or not the alarm was set successfully
     */
    virtual EStatus setAlarm(const clock::rtc_alarm_t& tm, clock::EAlarm alarm) = 0;

private:
    RtcHwInterface(const RtcHwInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcHwInterface& operator=(const RtcHwInterface&) = delete;      //! Delete copy constructors explicitly
    RtcHwInterface(const RtcHwInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcHwInterface&& operator=(const RtcHwInterface&&) = delete;    //! Delete move  constructors explicitly
};
