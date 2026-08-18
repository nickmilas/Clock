/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief An alias to make it easier for Event Queues to pass lambdas
*/

#pragma once

#include <functional>
#include <vector>

template <typename event>
class Publisher
{
public:
    /** @brief Helper alias to pass along notifications */
    using func = std::function<void(event&)>;

    /** @brief Function to notify all listeners */
    void Notify(func ev)
    {
        for (event* listener : mListeners)
        {
            if (listener != nullptr)
            {
                ev(*listener);
            }
        }
    }

    /** @brief Function to register for RtcHwEvents */
    void Register(event& listener) { mListeners.push_back(&listener); }
private:
    /** @brief List of all components listening */
    std::vector<event*> mListeners;
};