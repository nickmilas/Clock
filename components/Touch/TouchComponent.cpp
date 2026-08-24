/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Declaration of the TouchComponent class.
*/

#include "TouchComponent.hpp"
#include <cassert>

TouchComponent::TouchComponent(TouchHwInterface& touchHw) :
    TouchComponentInterface(),
    TouchHwEvent(),
    mTouchHw{touchHw}
{
    assert(xTaskCreate(TouchComponent::taskFunction, "TCH_COMP", 4096U, static_cast<void*>(this), (tskIDLE_PRIORITY + 1), &mHandle) == pdPASS);
    printf("TCH_COMP: Successfully created task.\n\n");

    mTouchHw.Register(*this);
}

void TouchComponent::eventTouchDetected()
{
    BaseType_t highPriorityWoken{pdFALSE};
    vTaskNotifyGiveFromISR(mHandle, &highPriorityWoken);
    portYIELD_FROM_ISR(highPriorityWoken);
}

void TouchComponent::taskFunction(void* pArgs)
{
    if (pArgs == nullptr)
    {
        printf("TCH_COMP: Task args invalid...\n");
        return;
    }

    static TouchComponent* self{static_cast<TouchComponent*>(pArgs)};
    while (true)
    {
        //vTaskDelay(pdMS_TO_TICKS(1000U)); // 1 second intervals
        /* Wait until we get our interrupt from the rtc before checking for which alarm expired */
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(portMAX_DELAY)))
        {
            if (self->mTouchHw.readTouch(self->mCurrX, self->mCurrY) == EStatus::Success)
            {
                printf("TCH_COMP: Touch coordinates are: (X, Y)\n");
            }
        }
    }
}
