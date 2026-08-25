/*
 * Copyright 2026 - Nicholas Milas Clk project
 * @brief Definition for the CH422 device.
*/

#include "CH422.hpp"
#include "I2CBusMaster.hpp"
#include <cassert>

CH422::CH422(I2CBusInterface& i2cBus) :
    ExioInterface(),
    mI2CBus{i2cBus}
{
    assert(mI2CBus.addDevice(&mSysCmdConfig, mSysCmdHandle) == EStatus::Success);
    printf("CH422: Successfully added device with address: 0x%x\n", smSysCmdAddr);

    assert(mI2CBus.addDevice(&mWriteConfig, mWriteHandle) == EStatus::Success);
    printf("CH422: Successfully added device with address: 0x%x\n", smWriteAddr);

    assert(mI2CBus.addDevice(&mReadConfig, mReadHandle) == EStatus::Success);
    printf("CH422: Successfully added device with address: 0x%x\n", smReadAddr);

    assert(setMode() == EStatus::Success);
    printf("CH422: Successfully configured device.\n\n");
}

EStatus CH422::setHigh(EEXIO pin)
{
    uint8_t pinOutputs{0U};
    EStatus status{readPinOutputs(pinOutputs)};
    if (status != EStatus::Success)
    {
        printf("CH422:: Failed to read exio pin values!\n");
    }

    uint8_t pinToSet{static_cast<uint8_t>(1U << static_cast<uint8_t>(pin))};
    pinOutputs |= pinToSet;

    status = mI2CBus.write(smWriteAddr, &pinOutputs, sizeof(uint8_t));
    if (status != EStatus::Success)
    {
        printf("CH422: Failed to set exio pin '%d' value HIGH!\n", pinToSet);
    }

    return status;
}

EStatus CH422::setLow(EEXIO pin)
{
    uint8_t pinOutputs{0U};
    EStatus status{readPinOutputs(pinOutputs)};
    if (status != EStatus::Success)
    {
        printf("CH422:: Failed to read exio pin values!\n");
    }

    uint8_t pinToSet{static_cast<uint8_t>(1U << static_cast<uint8_t>(pin))};
    pinOutputs &= ~(pinToSet);

    status = mI2CBus.write(smWriteAddr, &pinOutputs, sizeof(uint8_t));
    if (status != EStatus::Success)
    {
        printf("CH422: Failed to set exio pin '%d' value HIGH!\n", pinToSet);
    }

    return status;
}

EStatus CH422::setMode()
{
    uint8_t mode{0b00000001};
    EStatus status{mI2CBus.write(smSysCmdAddr, &mode, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("CH422: Failed to set the mode of the device!\n");
    }

    return status;
}

EStatus CH422::readPinOutputs(uint8_t& pinOutputs)
{
    EStatus status{mI2CBus.read(smReadAddr, &pinOutputs, sizeof(uint8_t))};
    if (status != EStatus::Success)
    {
        printf("CH422: Failed to read the pin values of the device!\n");
    }

    return status;
}
