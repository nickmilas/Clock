extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_lvgl_port.h"
}

#include "clock_enums.hpp"
#include "DS3231.hpp"
#include "I2CBusMaster.hpp"
#include "RtcComponent.hpp"

// Need to extern "C" here so that the esp-idf build tool can find our main
extern "C" void app_main(void)
{
    I2CBusMaster bus;
    DS3231 rtc{bus};
    RtcComponent rtcComp{rtc};

    clock::rtc_time_t tm{59U, 59U, 23U, 1U, 17U, 8U, 26U};
    std::ignore = rtc.setTime(tm);

    clock::rtc_alarm_t alarm{.min = 1U, .hour = 0};
    if (rtc.setAlarm(alarm, clock::EAlarm::Timer) == EStatus::Success)
    {
        printf("Set alarm for one minutes ahead!\n");
    }

    while (1) // This guy (our main/startup task) was keeping the watchdog from getting woken up 
    {
        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
    }
}
