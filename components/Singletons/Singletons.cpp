/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Definition for all Singletons that need to be constructed at startup.
*/

#include "Singletons.hpp"

I2CBusInterface& Singletons::getI2CBus()
{
    static I2CBusMaster i2c;
    return i2c;
}

ExioInterface& Singletons::getExioHw()
{
    static CH422 exio{getI2CBus()};
    return exio;
}

RtcHwInterface& Singletons::getRtcHw()
{
    static DS3231 rtcHw{getI2CBus()};
    return rtcHw;
}

RtcComponentInterface& Singletons::getRtcComp()
{
    static RtcComponent rtc{getRtcHw()};
    return rtc;
}

TouchHwInterface& Singletons::getTouchHw()
{
    static GT911 touchHw{getI2CBus()};
    return touchHw;
}

TouchComponentInterface& Singletons::getTouchComp()
{
    static TouchComponent touch{getTouchHw()};
    return touch;
}

Singletons::Singletons()
{
    // INITIALIZATION ORDER MATTERS HERE!
    std::ignore = getI2CBus();
    std::ignore = getExioHw();
    std::ignore = getRtcHw();
    std::ignore = getRtcComp();
    std::ignore = getTouchHw();
    std::ignore = getTouchComp();
}
