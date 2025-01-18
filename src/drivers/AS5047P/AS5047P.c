#include "AS5047P.h"

#include <stddef.h>

static AS5047PInterface interface = { NULL };


AS5047PStatus AS5047P_SetInterface( AS5047PInterface inter )
{
    if ( inter.spiWrite && inter.spiRead )
    {
        interface.spiWrite = inter.spiWrite;
        interface.spiRead  = inter.spiRead;

        return AS5047P_INTERFACE_SET;
    }

    return AS5047P_INTERFACE_UNSET;
}


static int isAddressReadOnly( uint16_t address )
{
    return ( ( address < ZPOSM || address > SETTINGS2 ) && address != PROG );
}


static void clearCmdFrame( uint32_t* data )
{
    *data &= ~( 0xFFFF0000 );
}


static void placeAddress( uint32_t* data, uint16_t address )
{
    // remove extra bits off address
    address &= ~( 0xC000 );
    // Place address at bit 16
    *data |= address << 16;
}


static void clearReadWriteBit( uint32_t* data )
{
    *data &= ~( 0x40000000 );
}


static void setReadWriteBit( uint32_t* data )
{
    *data |= 0x40000000;
}

static void clearParityAndEFBits( uint32_t* data )
{
    *data &= ~( 0x8000C000 );
}


static int isAddressOutOfBounds( uint16_t address )
{
    return ( ( address < ZPOSM   ||   address >  SETTINGS2 ) &&
             ( address < DIAAGC  ||   address >  ANGLECOM  ) &&
             ( address > ERRFL ) && ( address != PROG      )    );
}


void AS5047P_Write( uint32_t data, uint16_t address )
{
    // return if read only address
    if ( isAddressReadOnly( address ) )
        return;

    // Clear command frame portion
    clearCmdFrame( &data );
    
    placeAddress( &data, address );

    // Set r/w bit low
    clearReadWriteBit( &data ); 

    // Set parity and EF bits low
    clearParityAndEFBits( &data );

    interface.spiWrite( data );
}


uint32_t AS5047P_Read( uint16_t address )
{
    if ( isAddressOutOfBounds( address ) )
        return AS5047P_OUT_OF_BOUNDS_ADDRESS;

    uint32_t cmdOut = 0;

    placeAddress( &cmdOut, address );
    
    // Set r/w bit high
    setReadWriteBit( &cmdOut );

    // Set parity and EF bits low
    clearParityAndEFBits( &cmdOut );

    uint32_t readingBack = interface.spiRead( cmdOut );

    // Check if parity bits back are set, if not fail
    if ( ! ( ( readingBack & 0x80000000 ) && ( readingBack & 0x00008000 ) ) )
        return AS5047P_PARITY_FAIL;
    
    // Check if EF bit is set, if so fail
    if ( readingBack & 0x00004000 )
        return AS5047P_EF_FAIL;

    return readingBack;
}
