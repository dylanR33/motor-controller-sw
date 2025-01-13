#include "FakeRead.h"

#include <stdint.h>

static uint16_t nextRead = 0;
static uint16_t lastCmd = 0;

void FakeRead_Create()
{
    nextRead = 0;
    lastCmd = 0;
}

void FakeRead_SetNextReading(uint16_t fakeReading)
{
    nextRead = fakeReading;
}

uint16_t FakeRead_GetLastCmd()
{
    return lastCmd;
}

uint16_t FakeRead_Read(uint16_t cmd)
{
    lastCmd = cmd;
    return nextRead;
}
