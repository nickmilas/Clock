/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Namespace to store all clock related structs, enums, arrays, etc.
*/
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace clock {
    /** @brief Struct to represent the seconds of time stored and read from the rtc */
    typedef struct {
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
        uint8_t day;
        uint8_t date;
        uint8_t month;
        uint8_t year;
    } rtc_time_t;

    /** @brief Struct to represent an alarm stored within the rtc */
    typedef struct {
        uint8_t min;
        uint8_t hour;
        bool isShortTimer; // Under an hour
    } rtc_alarm_t;

    /** @brief Enum class to represent whether we are setting an alarm or a timer */
    enum class EAlarm : uint8_t {
        Timer = 0,
        Alarm,

        Size
    };

    /** @brief Enum class to respresent the days of the week */
    enum class day_t : uint8_t {
        Sunday = 0,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,

        Size
    };

    /** @brief Map that uses the day-of-the-week enum as a key, and a string as the value */
    const std::unordered_map<day_t, std::string> day_to_string {
        {day_t::Sunday,     "Sunday"},
        {day_t::Monday,     "Monday"},
        {day_t::Tuesday,    "Tuesday"},
        {day_t::Wednesday,  "Wednesday"},
        {day_t::Thursday,   "Thursday"},
        {day_t::Friday,     "Friday"},
        {day_t::Saturday,   "Saturday"}
    };
};
