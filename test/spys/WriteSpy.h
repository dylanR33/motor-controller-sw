#ifndef __WRITE_SPY_H__
#define __WRITE_SPY_H__

#include <stdint.h>

void WriteSpy_Create();

uint16_t WriteSpy_GetLastWrite16();

uint32_t WriteSpy_GetLastWrite32();

void WriteSpy_Write16( uint16_t cmd );

void WriteSpy_Write32( uint32_t cmd );

#endif

