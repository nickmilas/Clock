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
     * @brief Structure used to keep track of time/calender data
     */

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
     * @return Status_t - Whether or not the read was successful
     */
    virtual Status_t getTime(clock::rtc_time_t& tm) = 0;

    /**
     * @brief Set the Time object
     * 
     * @param tm - The time to set on the RTC
     * @return Status_t - Whether or not the write was successful
     */
    virtual Status_t setTime(const clock::rtc_time_t& tm) = 0;

private:
    RtcHwInterface(const RtcHwInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcHwInterface& operator=(const RtcHwInterface&) = delete;      //! Delete copy constructors explicitly
    RtcHwInterface(const RtcHwInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcHwInterface&& operator=(const RtcHwInterface&&) = delete;    //! Delete move  constructors explicitly
};
