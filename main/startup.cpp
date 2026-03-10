#include "DS3231.hpp"
#include "I2CBusMaster.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Need to extern "C" here so that the esp-idf build tool can find our main
extern "C" void app_main(void)
{
    I2CBusMaster bus;
    DS3231 rtc{bus};

    rtc_time_t tm{};
    tm.sec = 10U;
    tm.min = 48U;
    tm.hour = 9U;
    tm.day = 1U;
    tm.date = 8U;
    tm.month = 3U;
    tm.year = 26U;

    Status_t setStatus{rtc.setTime(tm)};
    if (setStatus == Status_t::Success)
    {
        printf("Time is set to %d/%d/%d - %d:%d:%d\n", tm.month, tm.date, tm.year, tm.hour, tm.min, tm.sec);
    }
    else
    {
        printf("Failed to set time!\n");
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
        rtc_time_t readTime{};
        Status_t getStatus{rtc.getTime(readTime)};
        if (getStatus == Status_t::Success)
        {
            printf("Time is read is %d/%d/%d - %d:%d:%d\n", readTime.month, readTime.date, readTime.year, readTime.hour, readTime.min, readTime.sec);
        }
        else
        {
            printf("Failed to get time!\n");
        }
    }
}
