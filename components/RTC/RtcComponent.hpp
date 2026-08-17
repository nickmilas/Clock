/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Definition of the RTCComponent class. This class will wrap an RTC chip.
*/

#pragma once

extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/queue.h"
}

#include "RtcComponentInterface.hpp"
#include "RtcHwInterface.hpp"

class RtcComponent : public RtcComponentInterface,
                     public RtcHwEvent
{
public:
    /**
     * @brief Constructor for the RtcComponent.
     * 
     * @param rtc - Reference to the rtc hardware chip
     */
    RtcComponent(RtcHwInterface& rtc);

    /**
     * @brief Default destructor.
     */
    ~RtcComponent() override = default;

    /** @copydoc RtcComponentInterface::setTime */
    EStatus setTime(const clock::rtc_time_t& tm) override;
    /** @copydoc RtcComponentInterface::getTime */
    EStatus getTime(clock::rtc_time_t& tm) override;
    /** @copydoc RtcHwEvent::eventAlarmExpired */
    void eventAlarmExpired() override;

    /**
     * @brief Function used to command the rtc chip to clear the expired alarm flags
     */
    EStatus clearExpiredFlags(bool& isTimerExpired);

private:
    /**
     * @brief Helper function to fire off our alarm expired event
     *  -> Marking function as static so it does not belong to an RtcComponent and can match the void (*) (void* arg) signature
     * 
     * @param args - We pass ourself as the argument
     */
    static void taskFunction(void* pArgs);

    /** @brief Handle for the current task */
    TaskHandle_t mHandle;

    /** @brief Reference to the rtc hardware */
    RtcHwInterface& mRtc;

    RtcComponent(const RtcComponent&) = delete;                 //! Delete copy-assignment constructors explicitly
    RtcComponent& operator=(const RtcComponent&) = delete;      //! Delete copy constructors explicitly
    RtcComponent(const RtcComponent&&) = delete;                //! Delete move-assignment constructors explicitly
    RtcComponent&& operator=(const RtcComponent&&) = delete;    //! Delete move  constructors explicitly
};
