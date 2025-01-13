#ifndef __FAKE_READ_H__
#define __FAKE_READ_H__

#include <stdint.h>

void FakeRead_Create();

void FakeRead_SetNextReading(uint16_t reading);

uint16_t FakeRead_GetLastCmd();

uint16_t FakeRead_Read(uint16_t cmd);

#endif

