/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Interface for a TouchComponent.
*/

#pragma once

#include "Status.hpp"

class TouchComponentInterface
{
public:
    /**
     * @brief Default constructor.
     */
    TouchComponentInterface() = default;

    /**
     * @brief Default destructor.
     */
    virtual ~TouchComponentInterface() = default;

private:
    TouchComponentInterface(const TouchComponentInterface&) = delete;                 //! Delete copy-assignment constructors explicitly
    TouchComponentInterface& operator=(const TouchComponentInterface&) = delete;      //! Delete copy constructors explicitly
    TouchComponentInterface(const TouchComponentInterface&&) = delete;                //! Delete move-assignment constructors explicitly
    TouchComponentInterface&& operator=(const TouchComponentInterface&&) = delete;    //! Delete move  constructors explicitly
};
