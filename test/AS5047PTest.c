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

TEST( AS5047P, ReadEnsuresParityBitsBackAreSet )
{
    FakeRead_SetNextReading32( 0x000000FF );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    FakeRead_SetNextReading32( 0x000080FF );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    FakeRead_SetNextReading32( 0x800000FF );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );
}

TEST( AS5047P, ReadEnsuresEFBackIsLow )
{
    FakeRead_SetNextReading32( 0x8000C0FF );
    TEST_ASSERT_EQUAL( AS5047P_EF_FAIL, AS5047P_Read( PROG ) );
}

TEST( AS5047P, SetPROGMemberBitsPlacedCorrectly )
{
    AS5047PPROG cfg =
    {
        .progver = 1,
        .progotp = 1,
        .otpref  = 0,
        .progen  = 1
    };
    AS5047P_SetPROG( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x00000049, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetZPOSMMemberBitsPlacedCorrectly )
{
    AS5047PZPOSM cfg =
    {
        .zposMSB = 15
    };
    AS5047P_SetZPOSM( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x0000000F, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetZPOSLMemberBitsPlacedCorrectly )
{
    AS5047PZPOSL cfg =
    {
        .zposLSB         = 17,
        .comp_l_error_en = 1,
        .comp_h_error_en = 0
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x0000051, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetSETTINGS1MemberBitsPlacedCorrectly )
{
    AS5047PSETTINGS1 cfg =
    {
        .dir             = 1,
        .uvw_abi         = 0,
        .daecdis         = 1,
        .abibin          = 0,
        .dataselect      = 1,
        .pwmOn           = 0
    };
    AS5047P_SetSETTINGS1( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x00000054, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetSETTINGS2MemberBitsPlacedCorrectly )
{
    AS5047PSETTINGS2 cfg =
    {
        .uvwpp  = 6,
        .hys    = 2,
        .abires = 5
    };
    AS5047P_SetSETTINGS2( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x000000B6, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetPROGFailIfMembersBitsExceedExpected )
{
    AS5047PPROG cfg =
    {
        .progver = 5,
        .progotp = 1,
        .otpref  = 4,
        .progen  = 100 
    };
    AS5047P_SetPROG( &cfg );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}

// Not needed since zposm is 8 bits and the member is uint8_t
// so it will not exceed
//TEST( AS5047P, SetZPOSMFailIfMembersBitsExceedExpected )
//{
//}

TEST( AS5047P, SetZPOSLFailIfMembersBitsExceedExpected )
{
    AS5047PZPOSL cfg =
    {
        .zposLSB = 200,
        .comp_l_error_en = 2,
        .comp_h_error_en = 2 
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetSETTINGS1FailIfMembersBitsExceedExpected )
{
    AS5047PSETTINGS1 cfg =
    {
        .dir             = 39,
        .uvw_abi         = 9,
        .daecdis         = 1,
        .abibin          = 8,
        .dataselect      = 1,
        .pwmOn           = 0
    };
    AS5047P_SetSETTINGS1( &cfg );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}

TEST( AS5047P, SetSETTINGS2FailIfMembersBitsExceedExpected )
{
    AS5047PSETTINGS2 cfg =
    {
        .uvwpp  = 6,
        .hys    = 5,
        .abires = 8
    };
    AS5047P_SetSETTINGS2( &cfg );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite32() );
}

// if above is implemented then the following shouldn't be necessary
// since not possible for bits to bleed into surrounding bit areas
//
//TEST( AS5047P, SetPROGNonSettableBitsLow )
//{
//}
//
//TEST( AS5047P, SetZPOSMNonSettableBitsLow )
//{
//}
//
//TEST( AS5047P, SetZPOSLNonSettableBitsLow )
//{
//}
//
//TEST( AS5047P, SetSETTINGS1NonSettableBitsLow )
//{
//}
//
//TEST( AS5047P, SetSETTINGS2NonSettableBitsLow )
//{
//}

