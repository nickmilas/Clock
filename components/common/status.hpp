#pragma once

#include <cstdint>

/** @brief Enum to get abstracted away from esp-idf return codes */
enum Status_t : uint8_t
{
    Success = 0U,        // Successful operation
    Error,              // Generic catch-all
    Range,              // Out-of-range
    Timeout,            // Operation took too long
    Memory,             // Not enough memory or memory related error (something nullptr for example)

    Size                // Number of status types
};
