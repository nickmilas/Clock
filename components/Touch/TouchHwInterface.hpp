/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Declaration for the TouchHwInterface driver.
*/

#pragma once

extern "C"
{
    #include "driver/gpio.h"
    #include "esp_attr.h"
}

#include "Status.hpp"
#include "Publisher.hpp"

class TouchHwEvent
{
public:
    /**
     * @brief Default constructor.
     */
    TouchHwEvent() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~TouchHwEvent() = default;

    /**
     * @brief Event received from the hardware notifying of an alarm/timer expiration
     */
    virtual void eventTouchDetected() = 0;

private:
    TouchHwEvent(const TouchHwEvent&) = delete;                 //! Delete copy-assignment constructors explicitly
    TouchHwEvent& operator=(const TouchHwEvent&) = delete;      //! Delete copy constructors explicitly
    TouchHwEvent(const TouchHwEvent&&) = delete;                //! Delete move-assignment constructors explicitly
    TouchHwEvent&& operator=(const TouchHwEvent&&) = delete;    //! Delete move  constructors explicitly
};

class TouchHwInterface : public Publisher<TouchHwEvent>
{
public:
    /**
     * @brief Default constructor
     */
    TouchHwInterface() = default;;

    /**
     * @brief Default destructor.
     */
    virtual ~TouchHwInterface() = default;

    /**
     * @brief Function responsible for reading touch from a controller
     */
    virtual EStatus readTouch(uint16_t& x, uint16_t& y) = 0;

private:
    TouchHwInterface(const TouchHwInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    TouchHwInterface& operator=(const TouchHwInterface&) = delete;      //! Delete copy constructors explicitly
    TouchHwInterface(const TouchHwInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    TouchHwInterface&& operator=(const TouchHwInterface&&) = delete;    //! Delete move  constructors explicitly
};
