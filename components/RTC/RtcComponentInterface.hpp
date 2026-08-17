/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for an RTCComponent.
*/

#pragma once

#include "clock_enums.hpp"
#include "status.hpp"

class RtcComponentInterface
{
public:
    /**
     * @brief Default constructor.
     */
    RtcComponentInterface() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~RtcComponentInterface() = default;

    /**
     * @brief Set the time through the hardware
     * 
     * @param tm - The time to set on the RTC
     * @return EStatus - Whether or not the write was successful
     */
    virtual EStatus setTime(const clock::rtc_time_t& tm) = 0;

    /**
     * @brief Read the time off of the hardware
     * 
     * @param tm - The time read off of the RTC
     * @return EStatus - Whether or not the read was successful
     */
    virtual EStatus getTime(clock::rtc_time_t& tm) = 0;

private:
    RtcComponentInterface(const RtcComponentInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcComponentInterface& operator=(const RtcComponentInterface&) = delete;      //! Delete copy constructors explicitly
    RtcComponentInterface(const RtcComponentInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcComponentInterface&& operator=(const RtcComponentInterface&&) = delete;    //! Delete move  constructors explicitly
};
