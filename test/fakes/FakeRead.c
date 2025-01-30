#include "FakeRead.h"

#include <stdint.h>

static uint16_t nextRead16 = 0;
static uint16_t lastCmd16  = 0;

static uint32_t nextRead32 = 0;
static uint32_t lastCmd32  = 0;

static uint32_t nextRead32VoidParameter = 0;

void FakeRead_Create()
{
    nextRead16 = 0;
    lastCmd16  = 0;

    nextRead32 = 0;
    lastCmd32  = 0;
}

void FakeRead_SetNextReading16(uint16_t fakeReading)
{
    nextRead16 = fakeReading;
}

void FakeRead_SetNextReading32(uint32_t fakeReading)
{
    nextRead32 = fakeReading;
}

void FakeRead_SetNextReading32VoidParameter( uint32_t fakeReading )
{
    nextRead32VoidParameter = fakeReading;
}

uint16_t FakeRead_GetLastCmd16()
{
    return lastCmd16;
}

uint32_t FakeRead_GetLastCmd32()
{
    return lastCmd32;
}

uint16_t FakeRead_Read16(uint16_t cmd)
{
    lastCmd16 = cmd;
    return nextRead16;
}

uint32_t FakeRead_Read32(uint32_t cmd)
{
    lastCmd32 = cmd;
    return nextRead32;
}

uint32_t FakeRead_Read32VoidParameter()
{
    return nextRead32VoidParameter;
}
