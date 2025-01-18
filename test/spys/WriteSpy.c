#include "WriteSpy.h"

static uint16_t lastWrite16 = 0;
static uint32_t lastWrite32 = 0;

void WriteSpy_Create()
{
    lastWrite16 = 0;
    lastWrite32 = 0;
}

uint16_t WriteSpy_GetLastWrite16()
{
    return lastWrite16;
}

uint32_t WriteSpy_GetLastWrite32()
{
    return lastWrite32;
}

void WriteSpy_Write16( uint16_t cmd )
{
    lastWrite16 = cmd;
}

void WriteSpy_Write32( uint32_t cmd )
{
    lastWrite32 = cmd;
}
