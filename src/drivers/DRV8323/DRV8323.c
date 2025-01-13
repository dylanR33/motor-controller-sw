#include "DRV8323.h"

#include <stdio.h>

static DRV8323Interface interface = {NULL};

enum 
{
    FAULT_STATUS_REG,
    VSG_STAUS_REG,
    RW_BIT = 0x8000,
    ADDRESS_UPPER_BOUND = 0x06,
    ADDRESS_START_BIT = 11,
    NON_DATA_BITS = 0xF800
};

static int isAddressOutOfRange( uint8_t address )
{
    return address > ADDRESS_UPPER_BOUND;
}

static int isInterfaceValid( DRV8323Interface inter )
{
    return ( inter.spiRead && inter.spiWrite );
}

static int isAddressReadOnly( uint8_t address )
{
    return ( address == FAULT_STATUS_REG || address == VSG_STAUS_REG );
}

static void clearNonDataBits( uint16_t* data )
{
    *data &= ~NON_DATA_BITS;
}

static void placeAddress( uint16_t* data, uint8_t address )
{
    *data |= ( address << ADDRESS_START_BIT );
}

static void clearReadWriteBit( uint16_t* data )
{
    *data &= ~RW_BIT;
}

static void setReadWriteBit( uint16_t* data )
{
    *data |= RW_BIT;
}

DRV8323Status DRV8323_SetInterface( DRV8323Interface inter )
{
    if ( isInterfaceValid( inter ) )
    {
        interface = inter;
        return DRV8323_INTERFACE_SET;
    }

    return DRV8323_INTERFACE_UNSET;
}


void DRV8323_Write( uint16_t data, uint8_t address )
{
    if ( isAddressOutOfRange( address ) )
        return;

    if ( isAddressReadOnly( address ) )
        return;

    clearNonDataBits( &data );

    placeAddress( &data, address );

    clearReadWriteBit( &data );

    interface.spiWrite( data );
}


uint16_t DRV8323_Read( uint8_t address ) 
{
    if (  isAddressOutOfRange( address ) )
        return OUT_OF_BOUNDS_ADDRESS;

    uint16_t cmdOut = 0;

    placeAddress( &cmdOut, address );

    setReadWriteBit( &cmdOut );

    return interface.spiRead( cmdOut );
}


