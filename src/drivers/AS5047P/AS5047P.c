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


void AS5047P_Write( uint32_t data, uint16_t address )
{
    // return if read only address
    if ( ( address < 0x0016 || address > 0x0019 ) && address != 0x0003 )
        return;

    // Clear command frame portion
    data &= ~( 0xFFFF0000 );
    
    // remove extra bits off address
    address &= ~( 0xC000 );
    // Place address at bit 16
    data |= address << 16;

    // Set r/w bit low
    data &= ~( 0x40000000 );
    
    // Set parity and EF bits low
    data &= ~( 0x8000C000 );

    interface.spiWrite( data );
}


uint32_t AS5047P_Read( uint16_t address )
{
    if ( ( address < 0x0016 || address > 0x0019 ) &&
         ( address < 0x3FFC || address > 0x3FFF ) &&
         ( address > 0x0001 ) &&
         ( address != 0x0003 )
       )
        return AS5047P_OUT_OF_BOUNDS_ADDRESS;

    uint32_t cmdOut = 0;

    // remove extra bits off address
    address &= ~( 0xC000 );
    // Place address at bit 16
    cmdOut |= address << 16;
    
    // Set r/w bit high
    cmdOut |= 0x40000000;

    // Set parity and EF bits low
    cmdOut &= ~( 0x8000C000 );

    return interface.spiRead( cmdOut );
}
