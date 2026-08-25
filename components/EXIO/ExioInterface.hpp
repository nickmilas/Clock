/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for the I/O extender driver.
*/

#pragma once

#include "Status.hpp"

class ExioInterface
{
public:
    enum class EEXIO
    {
        EXIO0 = 0,
        EXIO1, EXIO2, EXIO3,
        EXIO4, EXIO5, EXIO6, EXIO7
    };

    /**
     * @brief Default constructor
     */
    ExioInterface() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~ExioInterface() = default;

    /**
     * @brief Set a given pin to output a HIGH signal
     * 
     * @param pin Which pin to set HIGH
     */
    virtual EStatus setHigh(EEXIO pin) = 0;

    /**
     * @brief Set a given pin to output a LOW signal
     * 
     * @param pin Which pin to set LOW
     */
    virtual EStatus setLow(EEXIO pin) = 0;

private:
    ExioInterface(const ExioInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    ExioInterface& operator=(const ExioInterface&) = delete;      //! Delete copy constructors explicitly
    ExioInterface(const ExioInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    ExioInterface&& operator=(const ExioInterface&&) = delete;    //! Delete move  constructors explicitly
};
