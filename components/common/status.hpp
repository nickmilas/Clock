/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Enum class to represent a generic return status.
*/

#pragma once

#include <cstdint>

/** @brief Enum to get abstracted away from esp-idf return codes */
enum class Status_t : uint8_t
{
    Success = 0U,        // Successful operation
    Error,              // Generic catch-all
    Range,              // Out-of-range
    Timeout,            // Operation took too long
    Memory,             // Not enough memory or memory related error (something nullptr for example)

    Size                // Number of status types
};
