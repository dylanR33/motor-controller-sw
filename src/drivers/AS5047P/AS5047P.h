#ifndef __AS5047P_H__
#define __AS5047P_H__

#include <stdint.h>


typedef enum
{
    AS5047P_INTERFACE_SET,
    AS5047P_INTERFACE_UNSET
} AS5047PStatus;

enum
{
    AS5047P_OUT_OF_BOUNDS_ADDRESS
};


typedef struct
{
    void     ( *spiWrite )( uint32_t cmdOut );
    uint32_t ( *spiRead  )( uint32_t cmdOut );
} AS5047PInterface;


AS5047PStatus AS5047P_SetInterface( AS5047PInterface inter );

void AS5047P_Write( uint32_t data, uint16_t address );

uint32_t AS5047P_Read( uint16_t address );

#endif
