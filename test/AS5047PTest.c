#include "AS5047P.h"

#include <stdio.h>

#include "unity.h"
#include "unity_fixture.h"

#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( AS5047P );


TEST_SETUP( AS5047P )
{
    AS5047PInterface interface =
    {
        .spiWrite = WriteSpy_Write8Arr,
        .spiRead  = FakeRead_Read8Arr,
        .spiSetCS = WriteSpy_Write8
    };

    AS5047P_SetInterface( &interface );
    
    WriteSpy_Create( 10 );
    FakeRead_Create( 10, 10 );
}

TEST_TEAR_DOWN( AS5047P )
{
    WriteSpy_Destroy();
    FakeRead_Destroy();
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
    return ( uint32_t )( ( arr[ 0 ] << 24 ) | ( arr[ 1 ] << 16 ) | ( arr[ 2 ] << 8 ) | arr[ 3 ] );
}

TEST( AS5047P, WriteBit30Low )
{
    AS5047P_Write( 0xFFFF, ZPOSL );
    TEST_ASSERT_BIT_LOW( 30, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( AS5047P, ReadBit30and14High )
{
    AS5047P_Read( NOP );
    TEST_ASSERT_BIT_HIGH( 30, concatenate32( FakeRead_GetLastCmd8Arr() ) );
    TEST_ASSERT_BIT_HIGH( 14, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

TEST( AS5047P, PreventWriteToReadOnlyAddresses )
{
    AS5047P_Write( 0xFFFF, NOP );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite8ArrSize() );

    AS5047P_Write( 0xFFFF, DIAAGC );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite8ArrSize() );
}

TEST( AS5047P, WriteCmdOutParityAndEFCheck )
{
    AS5047P_Write( 0x0EFF, PROG );
    uint32_t lastWrite = concatenate32( WriteSpy_GetLastWrite8Arr() );
    // Parity checks
    TEST_ASSERT_BIT_HIGH( 31, lastWrite );
    TEST_ASSERT_BIT_LOW( 15, lastWrite );
    // EF check (should always be low for writes)
    TEST_ASSERT_BIT_LOW( 14, lastWrite );
}

TEST( AS5047P, ReadCmdOutParityCheck )
{
    AS5047P_Read( PROG );
    uint32_t lastCmd = concatenate32( FakeRead_GetLastCmd8Arr() );
    TEST_ASSERT_BIT_LOW( 31, lastCmd );
    TEST_ASSERT_BIT_LOW( 15, lastCmd );
}


TEST( AS5047P, PreventReadFromOutOfBoundsAddress )
{
    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( DIAAGC - 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd8ArrSize() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ZPOSM  - 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd8ArrSize() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ERRFL  + 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd8ArrSize() );
}

TEST( AS5047P, ReadEnsuresParityBitsMatch )
{
    uint8_t returnData[] = { 0x00, 0x0F };
    FakeRead_SetNextReading8Arr( returnData, sizeof( returnData ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );
}

TEST( AS5047P, ReadEnsuresEFBackIsLow )
{
    uint8_t returnData[] = { 0x40, 0x00 };
    FakeRead_SetNextReading8Arr( returnData, sizeof( returnData ) );
    TEST_ASSERT_EQUAL( AS5047P_EF_FAIL, AS5047P_Read( PROG ) );
}


TEST( AS5047P, SetZPOSLFullFrameCheck )
{
    uint8_t firstWrite[] = { 0x80, 0x17 };
    uint8_t secondWrite[] = { 0x00, 0x51 };

    uint8_t expectedWrite[] = { 0x80, 0x17, 0x00, 0x51 };

    AS5047Pzposl cfg =
    {
        .zposl         = 17,
        .comp_l_error_en = 1,
        .comp_h_error_en = 0
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_EQUAL_UINT8_ARRAY( expectedWrite, WriteSpy_GetLastWrite8Arr(), WriteSpy_GetLastWrite8ArrSize() );
}

