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
    tm.sec = 0U;
    tm.min = 7U;
    tm.hour = 8U;
    tm.day = 1U;
    tm.date = 23U;
    tm.month = 4U;
    tm.year = 26U;

    EStatus setStatus{rtc.setTime(tm)};
    if (setStatus == EStatus::Success)
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
        EStatus getStatus{rtc.getTime(readTime)};
        if (getStatus == EStatus::Success)
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
