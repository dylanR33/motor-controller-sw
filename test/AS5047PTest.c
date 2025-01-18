#include "AS5047P.h"

#include "unity.h"
#include "unity_fixture.h"

#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( AS5047P );


TEST_SETUP( AS5047P )
{
    AS5047PInterface interface =
    {
        .spiWrite = WriteSpy_Write32,
        .spiRead  = FakeRead_Read32
    };
    
    WriteSpy_Create();
    FakeRead_Create();
}

TEST_TEAR_DOWN( AS5047P )
{
}

TEST( AS5047P, SetInterface_RequireBothInterfaces )
{
    AS5047PInterface interface = 
    {
        .spiWrite = WriteSpy_Write32,
        .spiRead = NULL
    };

    TEST_ASSERT_EQUAL( AS5047P_INTERFACE_UNSET, AS5047P_SetInterface( interface ) );

    interface.spiRead = FakeRead_Read32;

    TEST_ASSERT_EQUAL( AS5047P_INTERFACE_SET, AS5047P_SetInterface( interface ) );
}

TEST( AS5047P, WriteBit30Low )
{
    AS5047P_Write( 0xFFFFFFFF, ZPOSL );
    TEST_ASSERT_BIT_LOW( 30, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, ReadBit30High )
{
    AS5047P_Read( NOP );
    TEST_ASSERT_BIT_HIGH( 30, FakeRead_GetLastCmd32() );
}

TEST( AS5047P, PreventWriteToReadOnlyAddresses )
{
    AS5047P_Write( 0xFFFFFFFF, NOP );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );

    AS5047P_Write( 0xFFFFFFFF, DIAAGC );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, WriteCmdOutParityAndEFBitsLow )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    TEST_ASSERT_BIT_LOW( 31, WriteSpy_GetLastWrite32() );
    TEST_ASSERT_BIT_LOW( 15, WriteSpy_GetLastWrite32() );
    TEST_ASSERT_BIT_LOW( 14, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, ReadCmdOutParityAndEFBitsLow )
{
    AS5047P_Read( PROG );
    TEST_ASSERT_BIT_LOW( 31, FakeRead_GetLastCmd32() );
    TEST_ASSERT_BIT_LOW( 15, FakeRead_GetLastCmd32() );
    TEST_ASSERT_BIT_LOW( 14, FakeRead_GetLastCmd32() );
}

TEST( AS5047P, ReadPlaceAddressBitsInBits29Through16 )
{
    AS5047P_Read( PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, FakeRead_GetLastCmd32() );
}

TEST( AS5047P, WritePlaceAddressBitsInBits29Through16 )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, RestrictDataBitsToBits13Through0 )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    TEST_ASSERT_EQUAL_HEX32( 0x00033FFF, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, ReadDataBitsCleared )
{
    AS5047P_Read( ZPOSM );
    TEST_ASSERT_BITS_LOW( 0x00003FFF, FakeRead_GetLastCmd32() );
}

TEST( AS5047P, PreventReadFromOutOfBoundsAddress )
{
    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( DIAAGC - 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd32() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ZPOSM  - 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd32() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ERRFL  + 1 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd32() );
}

