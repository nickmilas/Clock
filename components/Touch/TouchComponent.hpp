/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Definition of the TouchComponent class. This class will wrap a touch controller.
*/

#pragma once

extern "C"
{
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/queue.h"
}

#include "TouchComponentInterface.hpp"
#include "TouchHwInterface.hpp"

class TouchComponent : public TouchComponentInterface,
                       public TouchHwEvent
{
public:
    /**
     * @brief Constructor for the TouchComponent.
     * 
     * @param touchHw - Reference to the touch hardware
     */
    TouchComponent(TouchHwInterface& touchHw);

    /**
     * @brief Default destructor.
     */
    ~TouchComponent() override = default;

    /** @copydoc TouchHwEvent::eventTouchDetected */
    void eventTouchDetected() override;

private:
    /**
     * @brief Helper function to fire off our alarm expired event
     *  -> Marking function as static so it does not belong to an TouchComponent and can match the void (*) (void* arg) signature
     * 
     * @param args - We pass ourself as the argument
     */
    static void taskFunction(void* pArgs);

    /** @brief Handle for the current task */
    TaskHandle_t mHandle;
    /** @brief Reference to the rtc hardware */
    TouchHwInterface& mTouchHw;

    /** @brief Current x-coordinate read from hw */
    uint16_t mCurrX{0U};
    /** @brief Current y-coordinate read from hw */
    uint16_t mCurrY{0U};

    TouchComponent(const TouchComponent&) = delete;                 //! Delete copy-assignment constructors explicitly
    TouchComponent& operator=(const TouchComponent&) = delete;      //! Delete copy constructors explicitly
    TouchComponent(const TouchComponent&&) = delete;                //! Delete move-assignment constructors explicitly
    TouchComponent&& operator=(const TouchComponent&&) = delete;    //! Delete move  constructors explicitly
};
