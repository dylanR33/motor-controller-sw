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
        .spiWrite         = WriteSpy_Write8Arr,
        .spiRead          = FakeRead_Read8Arr,
        .spiSetCS         = WriteSpy_Write8,
        .adcReadRawPhaseA = FakeRead_Read32VoidParameter,
        .adcReadRawPhaseB = FakeRead_Read32VoidParameter,
        .adcReadRawPhaseC = FakeRead_Read32VoidParameter,
        .setEnablePin     = WriteSpy_Write8
    };
    DRV8323_SetInterface( &interface );

    WriteSpy_Create( 10 );

    FakeRead_Create( 10, 10);
}

TEST_TEAR_DOWN( DRV8323 )
{
    WriteSpy_Destroy();
    FakeRead_Destroy();
}


TEST( DRV8323, SetInterface_RequireAllInterfaces )
{
    DRV8323Interface interface = 
    {
        .spiWrite = WriteSpy_Write8Arr,
        .spiRead = NULL
    };
    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_UNSET, DRV8323_SetInterface( &interface ) );
    
    interface.spiRead = FakeRead_Read8Arr;
    interface.spiSetCS = WriteSpy_Write8;
    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_UNSET, DRV8323_SetInterface( &interface ) );

    interface.adcReadRawPhaseA = FakeRead_Read32VoidParameter;
    interface.adcReadRawPhaseB = FakeRead_Read32VoidParameter;
    interface.adcReadRawPhaseC = FakeRead_Read32VoidParameter;
    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_UNSET, DRV8323_SetInterface( &interface ) );

    interface.setEnablePin = WriteSpy_Write8;
    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_SET, DRV8323_SetInterface( &interface ) );
}

static uint16_t concatenate16( uint8_t* write )
{
    return ( uint16_t )( ( write[0] << 8 ) | write[1] );
}

TEST( DRV8323, WriteMSBLow )
{
    DRV8323_Write( 0xFFFF, 0x03 );
    TEST_ASSERT_BIT_LOW( 15, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( DRV8323, ReadMSBHigh )
{
    DRV8323_Read( 0x01 );
    TEST_ASSERT_BIT_HIGH( 15 , concatenate16( FakeRead_GetLastCmd8Arr() ) );
}

TEST( DRV8323, PreventWriteToOutOfBoundsAddresses )
{
    DRV8323_Write( 0xFFFF, 0x07 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite8ArrSize() );
}

TEST( DRV8323, PreventReadToOutOfBoundsAddresses )
{
    TEST_ASSERT_EQUAL( OUT_OF_BOUNDS_ADDRESS, DRV8323_Read( 0x08 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd8ArrSize() );
}

TEST( DRV8323, PreventWriteToReadOnlyAddresses )
{
    DRV8323_Write( 0xFFFF, 0x01 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite8ArrSize() );
}

TEST( DRV8323, RestrictDataToBits10Through0 )
{
    DRV8323_Write( 0xFFFF, 0x02 );
    TEST_ASSERT_EQUAL_HEX16( 0x17FF, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( DRV8323, ReadDataBitsCleared )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS_LOW( 0x07FF, concatenate16( FakeRead_GetLastCmd8Arr() ) );
}

TEST( DRV8323, WritePlaceAddressInBits14Through11 )
{
    DRV8323_Write( 0xFFFF, 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( DRV8323, ReadPlaceAddressInBits14Through11 )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, concatenate16( FakeRead_GetLastCmd8Arr() ) );
}

TEST( DRV8323, SetGateDriveHSFullFrameCheck )
{
    DRV8323gateDriveHS config = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };
    DRV8323_SetGateDriveHS( &config );
    TEST_ASSERT_EQUAL_HEX32( 0x1B9A, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( DRV8323, GetPhaseCurrentEquationProducesExpectedValue )
{

    FakeRead_SetNextReading32VoidParameter( 2000 );
    DRV8323CurrentSenseCfg iCfg =
    {
        .vRef = 3.3,
        .adcSteps = 4096,
        .rSense = 0.0015,
        .csaGain = 10
    };

    TEST_ASSERT_FLOAT_WITHIN( 0.001, 2.578, DRV8323_GetPhaseCurrentA( &iCfg ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, 2.578, DRV8323_GetPhaseCurrentB( &iCfg ) );
    TEST_ASSERT_FLOAT_WITHIN( 0.001, 2.578, DRV8323_GetPhaseCurrentC( &iCfg ) );

}

TEST( DRV8323, SetEnableStateSampleInput )
{
    DRV8323_SetEnableState( ENABLE_ON );
    TEST_ASSERT_EQUAL( 1, WriteSpy_GetLastWrite8() );

    DRV8323_SetEnableState( ENABLE_OFF );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite8() );
}
