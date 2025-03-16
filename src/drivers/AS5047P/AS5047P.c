#include "AS5047P.h"

#include <stddef.h>


static AS5047PInterface interface = { NULL };


// SPI Command Frame union containing bit fields which make up the frame
// as well as a raw frame
typedef union
{
    struct
    {
        uint16_t addr : 14;
        uint16_t rw : 1;
        uint16_t parc : 1;
    };
    uint16_t raw;
} CommandFrame;


// SPI Data Frame union containing bit fields which make up the frame
// as well as a raw frame
typedef union
{
    struct
    {
        uint16_t data : 14;
        uint16_t ef   : 1;
        uint16_t pard : 1;
    };
    uint16_t raw;
} DataFrame;


typedef enum
{
    CS_OFF,
    CS_ON
} CSState;


AS5047PStatus AS5047P_SetInterface( AS5047PInterface* inter )
{
    if ( inter->spiWrite && inter->spiRead && inter->spiSetCS )
    {
        interface = *inter;
        return AS5047P_INTERFACE_SET;
    }

    return AS5047P_INTERFACE_UNSET;
}


static int isAddressReadOnly( uint16_t address )
{
    return ( ( address < ZPOSM || address > SETTINGS2 ) && address != PROG );
}


static int isAddressOutOfBounds( uint16_t address )
{
    return ( ( address < ZPOSM   ||   address >  SETTINGS2 ) &&
             ( address < DIAAGC  ||   address >  ANGLECOM  ) &&
             ( address > ERRFL ) && ( address != PROG      )    );
}


// Checks if number of bits is even, if not the parity bit must be
// set to ensure even number of bits set
static uint8_t isEvenParity( uint16_t value )
{
    uint8_t setBitCount = 0;
    // Iterate over bits except MSB (parity bit)
    for ( uint8_t bitPos = 0; bitPos < 15; bitPos++ )
    {
        if ( value & ( 1 << bitPos ) )   setBitCount++;
    }

    // If set bits is even returns 1, else (odd) returns 0
    return ( ( setBitCount % 2 ) == 0 );
}


static void splitFrameIntoBuffer( uint16_t frame, uint8_t* buf )
{
    buf[ 0 ] = frame >> 8;
    buf[ 1 ] = frame;
}


void AS5047P_Write( uint16_t data, uint16_t address)
{
    // return if read only address
    if ( isAddressReadOnly( address ) )
        return;

    CommandFrame cmdFrame = { 0 };
    cmdFrame.addr = address;
    cmdFrame.rw = 0;
    if ( !isEvenParity( cmdFrame.raw ) )
        cmdFrame.parc = 1;

    DataFrame dataFrame = { 0 };
    dataFrame.data = data;
    if ( !isEvenParity( dataFrame.raw ) )
        dataFrame.pard = 1;

    uint8_t cmdOut[ sizeof( uint16_t ) ];
    splitFrameIntoBuffer( cmdFrame.raw , cmdOut );

    interface.spiSetCS( CS_OFF );
    interface.spiWrite( cmdOut, sizeof( cmdOut ) );
    interface.spiSetCS( CS_ON );

    uint8_t dataOut[ sizeof( uint16_t ) ];
    splitFrameIntoBuffer( dataFrame.raw , dataOut );

    interface.spiSetCS( CS_OFF );
    interface.spiWrite( dataOut, sizeof( dataOut ) );
    interface.spiSetCS( CS_ON );
}


static uint16_t extractBufferIntoFrame( uint8_t* buf )
{
    return ( uint16_t )( ( buf[ 0 ] << 8 ) | buf[ 1 ] );
}


uint16_t AS5047P_Read( uint16_t address )
{
    if ( isAddressOutOfBounds( address ) )
        return AS5047P_OUT_OF_BOUNDS_ADDRESS;

    CommandFrame cmdFrame = { 0 };
    cmdFrame.addr = address;
    cmdFrame.rw = 1;

    if ( !isEvenParity( cmdFrame.raw ) )
        cmdFrame.parc = 1;

    uint8_t cmdOut[ sizeof( uint16_t ) ];
    splitFrameIntoBuffer( cmdFrame.raw, cmdOut );

    uint8_t rxBuff[ sizeof( uint16_t ) ];

    interface.spiSetCS( CS_OFF );
    interface.spiRead( cmdOut, rxBuff, sizeof( cmdOut ) );
    interface.spiSetCS( CS_ON );

    interface.spiSetCS( CS_OFF );
    interface.spiRead( cmdOut, rxBuff, sizeof( cmdOut ) );
    interface.spiSetCS( CS_ON );

    DataFrame dataFrame = { 0 };

    dataFrame.raw = extractBufferIntoFrame( rxBuff );

    // Check if data parity matches calculated , if not fail
    if ( isEvenParity( dataFrame.raw ) && ( dataFrame.pard == 1 ) ) 
        return AS5047P_PARITY_FAIL;

    // Check if EF bit is set, if so fail
    if ( dataFrame.ef )
        return AS5047P_EF_FAIL;

    return dataFrame.data;
}


void AS5047P_SetPROG( AS5047Pprog* config )
{
    AS5047P_Write( config->raw, PROG );
}


void AS5047P_SetZPOSM( AS5047Pzposm* config )
{
    AS5047P_Write( config->raw, ZPOSM );
}


void AS5047P_SetZPOSL( AS5047Pzposl* config )
{
    AS5047P_Write( config->raw, ZPOSL );
}


void AS5047P_SetSETTINGS1( AS5047Psettings1* config )
{
    AS5047P_Write( config->raw, SETTINGS1 );
}


void AS5047P_SetSETTINGS2( AS5047Psettings2* config )
{
    AS5047P_Write( config->raw, SETTINGS2 );
}


void AS5047P_GetERRFL( AS5047Perrfl* errfl )
{
    errfl->raw = AS5047P_Read( ERRFL );
}


void AS5047P_GetPROG( AS5047Pprog* prog )
{
    prog->raw = AS5047P_Read( PROG );
}


void AS5047P_GetDIAAGC( AS5047Pdiaagc* diaagc )
{
    diaagc->raw = AS5047P_Read( DIAAGC );
}


void AS5047P_GetMAG( AS5047Pmag* mag )
{
    mag->raw = AS5047P_Read( MAG );
}


void AS5047P_GetANGLEUNC( AS5047Pangleunc* angleunc )
{
    angleunc->raw = AS5047P_Read( ANGLEUNC );
}


void AS5047P_GetANGLECOM( AS5047Panglecom* anglecom )
{
    anglecom->raw = AS5047P_Read( ANGLECOM );
}


void AS5047P_GetZPOSM( AS5047Pzposm* zposm )
{
    zposm->raw = AS5047P_Read( ZPOSM );
}


void AS5047P_GetZPOSL( AS5047Pzposl* zposl )
{
    zposl->raw = AS5047P_Read( ZPOSL );
}


void AS5047P_GetSETTINGS1( AS5047Psettings1* settings1 )
{
    settings1->raw = AS5047P_Read( SETTINGS1 );
}


void AS5047P_GetSETTINGS2( AS5047Psettings2* settings2 )
{
    settings2->raw = AS5047P_Read( SETTINGS2 );
}
