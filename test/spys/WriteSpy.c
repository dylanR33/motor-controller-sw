#include "WriteSpy.h"

static uint16_t lastWrite = 0;

void WriteSpy_Create()
{
    lastWrite = 0;
}

uint16_t WriteSpy_GetLastWrite()
{
    return lastWrite;
}

void WriteSpy_Write( uint16_t cmd )
{
    lastWrite = cmd;
}

