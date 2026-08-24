/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Kicking off the project. Initializing necessary stuffs.
*/

extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "DisplayStartup.h"
}

#include "ClockEnums.hpp"
#include "Singletons.hpp"

// Need to extern "C" here so that the esp-idf build tool can find our main
extern "C" void app_main(void)
{
    // Create all singletons
    Singletons s;
    static_cast<void>(s); // Throw this away, it is useless

    // Kick off lvgl
    startupLVGL();

    while (1) // This guy (our main/startup task) was keeping the watchdog from getting woken up 
    {
        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
    }
}
