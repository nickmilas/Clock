/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for a RTC hardware.
*/

#pragma once

#include "Status.hpp"
#include "ClockEnums.hpp"
#include "Publisher.hpp"
class RtcHwEvent
{
public:
    /**
     * @brief Default constructor.
     */
    RtcHwEvent() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~RtcHwEvent() = default;

    /**
     * @brief Event received from the hardware notifying of an alarm/timer expiration
     */
    virtual void eventAlarmExpired() = 0;

private:
    RtcHwEvent(const RtcHwEvent&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcHwEvent& operator=(const RtcHwEvent&) = delete;      //! Delete copy constructors explicitly
    RtcHwEvent(const RtcHwEvent&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcHwEvent&& operator=(const RtcHwEvent&&) = delete;    //! Delete move  constructors explicitly
};

class RtcHwInterface : public Publisher<RtcHwEvent>
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
     * @param alarm - Alarm or timer is to be set
     */
    virtual EStatus setAlarm(const clock::rtc_alarm_t& tm, clock::EAlarm alarm) = 0;

    /**
     * @brief Function to clear alarm flags
     * @param isTimerExpired - Flag indicating whether an alarm or timer expired
     */
    virtual EStatus clearExpiredFlags(bool& isTimerExpired) = 0;

    /**
     * @brief Disable either the alarm or timer
     * 
     * @param alarmType - Enum to decide which to disable
     */
    virtual EStatus disableAlarm(clock::EAlarm alarmType) = 0;

private:
    RtcHwInterface(const RtcHwInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcHwInterface& operator=(const RtcHwInterface&) = delete;      //! Delete copy constructors explicitly
    RtcHwInterface(const RtcHwInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcHwInterface&& operator=(const RtcHwInterface&&) = delete;    //! Delete move  constructors explicitly
};
