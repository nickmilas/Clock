/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Class to represent the DS3231 rtc chip.
 * 
 * This class will hold an I2C device object so that it can perform easy read a writes
*/

#pragma once

#include "RtcHwInterface.hpp"

class DS3231 : public IRtcHw
{
public:

private:
    // WHEN PERFORMING WRITE AND READ pData[0] WILL SPECIFY THE REGISTER WE WISH TO WRITE TO
    // IF YOU WANT TO WRITE A SINGLE REGISTER pData[0] IS THAT REGISTER
    // IF YOU WANT TO WRITE TO MULTIPLE REGISTERS pData[0] IS THE STARTING REGISTER, and WILL GO UNTIL IT REACHES THE END OF THE DATA
};
