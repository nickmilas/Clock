/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief An alias to make it easier for Event Queues to pass lambdas
*/

#pragma once

#include <functional>

template <typename T>
using lambda = std::function<void(T&)>;
