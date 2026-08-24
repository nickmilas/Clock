/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Declaration for all Singletons that need to be constructed at startup.
*/

#pragma once

#include "I2CBusMaster.hpp"

#include "DS3231.hpp"
#include "RtcComponent.hpp"

#include "GT911.hpp"
#include "TouchComponent.hpp"


class Singletons
{
public:
    /**
     * @brief Constructor for Singleton objects
     */
    Singletons();

    /**
     * @brief Default destructor (Should be unused)
     */
    ~Singletons() = default;

    /** @brief Fetch I2C bus */
    static I2CBusInterface& getI2CBus();

    /** @brief Fetch RtcHW */
    static RtcHwInterface& getRtcHw();
    /** @brief Fetch RtcComponent */
    static RtcComponentInterface& getRtcComp();

    /** @brief Fetch TouchHw */
    static TouchHwInterface& getTouchHw();
    /** @brief Fetch TouchComponent */
    static TouchComponentInterface& getTouchComp();

private:
    Singletons(const Singletons&) = delete;                 //! Delete copy-assignment constructors explicitly
    Singletons& operator=(const Singletons&) = delete;      //! Delete copy constructors explicitly
    Singletons(const Singletons&&) = delete;                //! Delete move-assignment constructors explicitly
    Singletons&& operator=(const Singletons&&) = delete;    //! Delete move  constructors explicitly
};
