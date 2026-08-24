/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief This file is responsible for starting up LVGL through esp-idf.
*/

#include "DisplayStartup.h"
#include "esp_lvgl_port.h"
#include "stdio.h"

void startupLVGL(void)
{
    const lvgl_port_cfg_t cfg = ESP_LVGL_PORT_INIT_CONFIG();
    // lvgl_port_init() launches lvgl itself, creates an internal task, and creates a
    // mutex to be used whenever interfacing with LVGL functions as they are not thread-safe
    assert(lvgl_port_init(&cfg) == ESP_OK); // Assert = no error
    printf("Successfully booted LVGL!\n");
}
