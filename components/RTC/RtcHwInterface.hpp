/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for a RTC component.
*/

#pragma once

#include "status.hpp"

class RtcHwInterface
{
public:
    /**
     * @brief Structure used to keep track of time/calender data
     */
    typedef struct {
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
        uint8_t day;
        uint8_t date;
        uint8_t month;
        uint8_t year;
    } time_t;

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
    virtual Status_t getTime(time_t& tm) = 0;

    /**
     * @brief Set the Time object
     * 
     * @param tm - The time to set on the RTC
     * @return Status_t - Whether or not the write was successful
     */
    virtual Status_t setTime(const time_t& tm) = 0;

private:
    RtcHwInterface(const RtcHwInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcHwInterface& operator=(const RtcHwInterface&) = delete;      //! Delete copy constructors explicitly
    RtcHwInterface(const RtcHwInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcHwInterface&& operator=(const RtcHwInterface&&) = delete;    //! Delete move  constructors explicitly
};
