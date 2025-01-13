#ifndef __WRITE_SPY_H__
#define __WRITE_SPY_H__

#include <stdint.h>

void WriteSpy_Create();

uint16_t WriteSpy_GetLastWrite();

void WriteSpy_Write( uint16_t cmd );
#endif

