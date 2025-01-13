#include "DRV8323.h"


#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( DRV8323 );


TEST_SETUP( DRV8323 )
{
    DRV8323Interface interface = 
    {
        .spiWrite = WriteSpy_Write,
        .spiRead = FakeRead_Read
    };
    DRV8323_SetInterface( interface );

    WriteSpy_Create();
    FakeRead_Create();
}

TEST_TEAR_DOWN( DRV8323 )
{
}


TEST( DRV8323,  SetInterface_RequireBothInterfaces)
{
    DRV8323Interface interface = 
    {
        .spiWrite = WriteSpy_Write,
        .spiRead = NULL
    };

    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_UNSET, DRV8323_SetInterface( interface ) );
    
    interface.spiRead = FakeRead_Read;

    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_SET, DRV8323_SetInterface( interface ) );
}

TEST( DRV8323, WriteMSBLow )
{
    DRV8323_Write( 0xFFFF, 0x03 );
    TEST_ASSERT_BIT_LOW( 15, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, ReadMSBHigh )
{
    DRV8323_Read( 0x01 );
    TEST_ASSERT_BIT_HIGH( 15 ,FakeRead_GetLastCmd() );
}

TEST( DRV8323, PreventWriteToOutOfBoundsAddresses )
{
    DRV8323_Write( 0xFFFF, 0x07 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, PreventReadToOutOfBoundsAddresses )
{
    TEST_ASSERT_EQUAL( OUT_OF_BOUNDS_ADDRESS, DRV8323_Read( 0x08 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd() );
}

TEST( DRV8323, PreventWriteToReadOnlyAddresses )
{
    DRV8323_Write( 0xFFFF, 0x01 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, RestrictDataToBits10Through0 )
{
    DRV8323_Write( 0xFFFF, 0x02 );
    TEST_ASSERT_EQUAL_HEX16( 0x17FF, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, ReadDataBitsCleared )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS_LOW( 0x07FF, FakeRead_GetLastCmd() );
}

TEST( DRV8323, WritePlaceAddressInBits14Through11 )
{
    DRV8323_Write( 0xFFFF, 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, ReadPlaceAddressInBits14Through11 )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, FakeRead_GetLastCmd() );
}


