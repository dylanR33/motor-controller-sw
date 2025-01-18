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


static int areParityBitsSet( uint32_t reading )
{
    return ( reading & 0x80000000 ) && ( reading & 0x00008000 );
}


static int isEFBitSet( uint32_t reading )
{
    return reading & 0x00004000;
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
    if ( !areParityBitsSet( readingBack ) )
        return AS5047P_PARITY_FAIL;
    
    // Check if EF bit is set, if so fail
    if ( isEFBitSet( readingBack ) )
        return AS5047P_EF_FAIL;

    return readingBack;
}


void AS5047P_SetPROG( AS5047PPROG* config )
{

    // Check if bits in members exceed expected
    if ( config->progver & ~0x01 ||
         config->progotp & ~0x01 ||
         config->otpref  & ~0x01 ||
         config->progen  & ~0x01    )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->progen  << 0
            |  config->otpref  << 2
            |  config->progotp << 3
            |  config->progver << 6;

    // Clear non-settable bits
    //dataOut &= ~( 0x00000000 );

    AS5047P_Write( dataOut, PROG );
}


void AS5047P_SetZPOSM( AS5047PZPOSM* config )
{
    // No need to check if bits exceeded since
    // zposm is 8 bits

    uint32_t dataOut = 0;

    dataOut |= config->zposMSB;

    AS5047P_Write( dataOut, ZPOSM );
}


void AS5047P_SetZPOSL( AS5047PZPOSL* config )
{
    // Check if bits in members exceed expected
    if ( config->zposLSB         & ~0x3F ||
         config->comp_l_error_en & ~0x01 ||
         config->comp_h_error_en & ~0x01    )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->zposLSB << 0
            |  config->comp_l_error_en << 6
            |  config->comp_h_error_en << 7;

    AS5047P_Write( dataOut, ZPOSL );
}


void AS5047P_SetSETTINGS1( AS5047PSETTINGS1* config )
{
    if ( config->dir & ~0x01 ||
         config->uvw_abi & ~0x01 ||
         config->daecdis & ~0x01 ||
         config->abibin & ~0x01 ||
         config->dataselect & ~0x01 ||
         config->pwmOn & ~0x01          )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->dir << 2
            |  config->uvw_abi << 3
            |  config->daecdis << 4
            |  config->abibin << 5
            |  config->dataselect << 6
            |  config->pwmOn << 7;

    AS5047P_Write( dataOut, SETTINGS1 );
}


void AS5047P_SetSETTINGS2( AS5047PSETTINGS2* config )
{
    if ( config->uvwpp & ~0x07 ||
         config->hys & ~0x03 ||
         config->abires & ~0x07 )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->uvwpp << 0
            |  config->hys << 3
            |  config->abires << 5;

    AS5047P_Write( dataOut, SETTINGS2 );
}
