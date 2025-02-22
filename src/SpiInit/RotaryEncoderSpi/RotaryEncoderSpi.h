#ifndef __ROTARY_ENCODER_SPI_H__
#define __ROTARY_ENCODER_SPI_H__

#include <stdint.h>

void RotaryEncoderSpi_Config();

void RotaryEncoderSpi_Write( uint32_t cmdOut );

uint32_t RotaryEncoderSpi_Read( uint32_t cmdOut );

void RotaryEncoderSpi_SetCS( uint8_t state );

#endif

