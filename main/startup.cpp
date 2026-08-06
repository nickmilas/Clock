#include "clock_enums.hpp"
#include "DS3231.hpp"
#include "I2CBusMaster.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Need to extern "C" here so that the esp-idf build tool can find our main
extern "C" void app_main(void)
{
    I2CBusMaster bus;
    DS3231 rtc{bus};

    clock::rtc_time_t tm{};
    tm.sec = 50U;
    tm.min = 59U;
    tm.hour = 11U;
    tm.day = 1U;
    tm.date = 8U;
    tm.month = 3U;
    tm.year = 26U;

    Status_t setStatus{rtc.setTime(tm)};
    if (setStatus == Status_t::Success)
    {
            printf("Time is read is %d/%d/%d - %d:%d:%d%s\n", tm.month,
                                                        tm.date,
                                                        tm.year,
                                                        tm.hour,
                                                        tm.min,
                                                        tm.sec,
                                                        (rtc.isAfternoon()) ? "pm" : "am");
    }
    else
    {
        printf("Failed to set time!\n");
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
        clock::rtc_time_t readTime{};
        Status_t getStatus{rtc.getTime(readTime)};
        if (getStatus == Status_t::Success)
        {
            printf("Time is read is %d/%d/%d - %d:%d:%d%s\n", readTime.month,
                                                        readTime.date,
                                                        readTime.year,
                                                        readTime.hour,
                                                        readTime.min,
                                                        readTime.sec,
                                                        (rtc.isAfternoon()) ? "pm" : "am");
        }
        else
        {
            printf("Failed to get time!\n");
        }
    }
}
