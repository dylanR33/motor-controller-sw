#include "AS5047P.h"

#include "unity.h"
#include "unity_fixture.h"

#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( AS5047P );


static uint8_t nextReading[ sizeof( uint16_t ) ] = { 0, 0 };

TEST_SETUP( AS5047P )
{
    AS5047PInterface interface =
    {
        .spiWrite = WriteSpy_Write8Arr,
        .spiRead  = FakeRead_Read8Arr,
        .spiSetCS = WriteSpy_Write8
    };

    AS5047P_SetInterface( &interface );
    
    WriteSpy_Create();

    FakeRead_Create();
    nextReading[ 0 ] = 0;
    nextReading[ 1 ] = 0;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
}

TEST_TEAR_DOWN( AS5047P )
{
}

TEST( AS5047P, SetInterface_RequireBothInterfaces )
{
    AS5047PInterface interface = 
    {
        .spiWrite = WriteSpy_Write8Arr,
        .spiRead = NULL
    };

    TEST_ASSERT_EQUAL( AS5047P_INTERFACE_UNSET, AS5047P_SetInterface( &interface ) );

    interface.spiRead = FakeRead_Read8Arr;
    interface.spiSetCS = WriteSpy_Write8;

    TEST_ASSERT_EQUAL( AS5047P_INTERFACE_SET, AS5047P_SetInterface( &interface ) );
}

static uint32_t concatenate32( uint8_t* arr )
{
    return ( uint32_t )( arr[ 0 ] << 24 ) | ( arr[ 1 ] << 16 ) | ( arr[ 2 ] << 8 ) | arr[ 3 ];
}

IGNORE_TEST( AS5047P, WriteBit30Low )
{
    AS5047P_Write( 0xFFFF, ZPOSL );
    TEST_ASSERT_BIT_LOW( 30, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

IGNORE_TEST( AS5047P, ReadBit30High )
{
    AS5047P_Read( NOP );
    TEST_ASSERT_BIT_HIGH( 30, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

IGNORE_TEST( AS5047P, PreventWriteToReadOnlyAddresses )
{
    AS5047P_Write( 0xFFFF, NOP );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );

    AS5047P_Write( 0xFFFF, DIAAGC );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

IGNORE_TEST( AS5047P, WriteCmdOutParityAndEFBitsLow )
{
    AS5047P_Write( 0xFFFF, PROG );
    uint32_t lastWrite = concatenate32( WriteSpy_GetLastWrite8Arr() );
    TEST_ASSERT_BIT_LOW( 31, lastWrite );
    TEST_ASSERT_BIT_LOW( 15, lastWrite );
    TEST_ASSERT_BIT_LOW( 14, lastWrite );
}

IGNORE_TEST( AS5047P, ReadCmdOutParityAndEFBitsLow )
{
    AS5047P_Read( PROG );
    uint32_t lastCmd = concatenate32( FakeRead_GetLastCmd8Arr() );
    TEST_ASSERT_BIT_LOW( 31, lastCmd );
    TEST_ASSERT_BIT_LOW( 15, lastCmd );
    TEST_ASSERT_BIT_LOW( 14, lastCmd );
}

IGNORE_TEST( AS5047P, ReadPlaceAddressBitsInBits29Through16 )
{
    AS5047P_Read( PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

IGNORE_TEST( AS5047P, WritePlaceAddressBitsInBits29Through16 )
{
    AS5047P_Write( 0xFFFF, PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

IGNORE_TEST( AS5047P, RestrictDataBitsToBits13Through0 )
{
    AS5047P_Write( 0xFFFF, PROG );
    TEST_ASSERT_EQUAL_HEX32( 0x00033FFF, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

IGNORE_TEST( AS5047P, ReadDataBitsCleared )
{
    AS5047P_Read( ZPOSM );
    TEST_ASSERT_BITS_LOW( 0x00003FFF, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

IGNORE_TEST( AS5047P, PreventReadFromOutOfBoundsAddress )
{
    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( DIAAGC - 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ZPOSM  - 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ERRFL  + 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );
}

IGNORE_TEST( AS5047P, ReadEnsuresParityBitsBackAreSet )
{
    nextReading[ 0 ] = 0x00;
    nextReading[ 1 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    nextReading[ 0 ] = 0x00;
    nextReading[ 1 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );
}

IGNORE_TEST( AS5047P, ReadEnsuresEFBackIsLow )
{
    nextReading[ 0 ] = 0xC0;
    nextReading[ 1 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_EF_FAIL, AS5047P_Read( PROG ) );
}


IGNORE_TEST( AS5047P, SetZPOSLFullFrameCheck )
{
    AS5047Pzposl cfg =
    {
        .zposl         = 17,
        .comp_l_error_en = 1,
        .comp_h_error_en = 0
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_EQUAL_HEX32( 0x00170051, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

