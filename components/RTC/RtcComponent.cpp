/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Implementation of the RTCComponent class. This class will wrap an RTC chip.
*/

extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
}

#include "RtcComponent.hpp"
#include <cassert>

RtcComponent::RtcComponent(RtcHwInterface& rtc) :
    RtcComponentInterface(),
    RtcHwEvent(),
    mRtcHw{rtc}
{
    assert(xTaskCreate(RtcComponent::taskFunction, "RTC_COMP", 4096U, static_cast<void*>(this), (tskIDLE_PRIORITY + 1), &mHandle) == pdPASS);
    printf("RTC_COMP: Successfully created task.\n\n");

    mRtcHw.Register(*this);
}

EStatus RtcComponent::setTime(const clock::rtc_time_t& tm)
{
    return mRtcHw.setTime(tm);
}

EStatus RtcComponent::getTime(clock::rtc_time_t& tm)
{
    return mRtcHw.getTime(tm);
}

EStatus RtcComponent::clearExpiredFlags(bool& isTimerExpired)
{
    return mRtcHw.clearExpiredFlags(isTimerExpired);
}

void RtcComponent::eventAlarmExpired()
{
    BaseType_t highPriorityWoken{pdFALSE};
    vTaskNotifyGiveFromISR(mHandle, &highPriorityWoken);
    portYIELD_FROM_ISR(highPriorityWoken);
}

void RtcComponent::taskFunction(void* pArgs)
{
    if (pArgs == nullptr)
    {
        printf("RTC_COMP: Task args invalid...\n");
        return;
    }

    static RtcComponent* self{static_cast<RtcComponent*>(pArgs)};
    while (true)
    {
//        vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
        /* Wait until we get our interrupt from the rtc before checking for which alarm expired */
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(portMAX_DELAY)))
        {
            bool isTimerExpired{false};
            if (self->clearExpiredFlags(isTimerExpired) != EStatus::Success)
            {
                printf("RTC_COMP: Failed to clear alarm flags!\n");
            }
            else
            {
                if (isTimerExpired)
                {
                    printf("RTC_COMP: Timer expired & disabled!\n");
                    self->mRtcHw.disableAlarm(clock::EAlarm::Timer);
                }
                else
                {
                    printf("RTC_COMP: Alarm expired!\n");
                }
            }
        }
    }
}
