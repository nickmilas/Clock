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

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} rtc_time_t;
