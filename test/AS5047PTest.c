#include "AS5047P.h"

#include "unity.h"
#include "unity_fixture.h"

#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( AS5047P );


static uint8_t nextReading[ sizeof( uint32_t ) ] = { 0, 0 };

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

TEST( AS5047P, WriteBit30Low )
{
    AS5047P_Write( 0xFFFFFFFF, ZPOSL );
    TEST_ASSERT_BIT_LOW( 30, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( AS5047P, ReadBit30High )
{
    AS5047P_Read( NOP );
    TEST_ASSERT_BIT_HIGH( 30, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

TEST( AS5047P, PreventWriteToReadOnlyAddresses )
{
    AS5047P_Write( 0xFFFFFFFF, NOP );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );

    AS5047P_Write( 0xFFFFFFFF, DIAAGC );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

TEST( AS5047P, WriteCmdOutParityAndEFBitsLow )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    uint32_t lastWrite = concatenate32( WriteSpy_GetLastWrite8Arr() );
    TEST_ASSERT_BIT_LOW( 31, lastWrite );
    TEST_ASSERT_BIT_LOW( 15, lastWrite );
    TEST_ASSERT_BIT_LOW( 14, lastWrite );
}

TEST( AS5047P, ReadCmdOutParityAndEFBitsLow )
{
    AS5047P_Read( PROG );
    uint32_t lastCmd = concatenate32( FakeRead_GetLastCmd8Arr() );
    TEST_ASSERT_BIT_LOW( 31, lastCmd );
    TEST_ASSERT_BIT_LOW( 15, lastCmd );
    TEST_ASSERT_BIT_LOW( 14, lastCmd );
}

TEST( AS5047P, ReadPlaceAddressBitsInBits29Through16 )
{
    AS5047P_Read( PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

TEST( AS5047P, WritePlaceAddressBitsInBits29Through16 )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    TEST_ASSERT_BITS( 0x3FFF0000, 0x00030000, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( AS5047P, RestrictDataBitsToBits13Through0 )
{
    AS5047P_Write( 0xFFFFFFFF, PROG );
    TEST_ASSERT_EQUAL_HEX32( 0x00033FFF, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( AS5047P, ReadDataBitsCleared )
{
    AS5047P_Read( ZPOSM );
    TEST_ASSERT_BITS_LOW( 0x00003FFF, concatenate32( FakeRead_GetLastCmd8Arr() ) );
}

TEST( AS5047P, PreventReadFromOutOfBoundsAddress )
{
    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( DIAAGC - 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ZPOSM  - 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );

    TEST_ASSERT_EQUAL( AS5047P_OUT_OF_BOUNDS_ADDRESS, AS5047P_Read( ERRFL  + 1 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );
}

TEST( AS5047P, ReadEnsuresParityBitsBackAreSet )
{
    nextReading[ 0 ] = 0x00;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x00;
    nextReading[ 3 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    nextReading[ 0 ] = 0x00;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x80;
    nextReading[ 3 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );

    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x00;
    nextReading[ 3 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    TEST_ASSERT_EQUAL( AS5047P_PARITY_FAIL, AS5047P_Read( PROG ) );
}

TEST( AS5047P, ReadEnsuresEFBackIsLow )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0xC0;
    nextReading[ 3 ] = 0xFF;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
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
    TEST_ASSERT_BITS( 0x00003FFF, 0x00000049, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( AS5047P, SetZPOSMMemberBitsPlacedCorrectly )
{
    AS5047PZPOSM cfg =
    {
        .zposMSB = 15
    };
    AS5047P_SetZPOSM( &cfg );
    TEST_ASSERT_BITS( 0x00003FFF, 0x0000000F, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x00003FFF, 0x0000051, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x00003FFF, 0x00000054, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x00003FFF, 0x000000B6, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

// Not needed since zposm is 8 bits and the member is uint8_t
// so it will not exceed
//IGNORE_TEST( AS5047P, SetZPOSMFailIfMembersBitsExceedExpected )
//{
//}

TEST( AS5047P, SetZPOSLFullFrameCheck )
{
    AS5047PZPOSL cfg =
    {
        .zposLSB         = 17,
        .comp_l_error_en = 1,
        .comp_h_error_en = 0
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_EQUAL_HEX32( 0x00170051, concatenate32( WriteSpy_GetLastWrite8Arr() ) );
}


TEST( AS5047P, SetZPOSLFailIfMembersBitsExceedExpected )
{
    AS5047PZPOSL cfg =
    {
        .zposLSB = 200,
        .comp_l_error_en = 2,
        .comp_h_error_en = 2 
    };
    AS5047P_SetZPOSL( &cfg );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

// if above is implemented then the following shouldn't be necessary
// since not possible for bits to bleed into surrounding bit areas
//
//IGNORE_TEST( AS5047P, SetPROGNonSettableBitsLow )
//{
//}
//
//IGNORE_TEST( AS5047P, SetZPOSMNonSettableBitsLow )
//{
//}
//
//IGNORE_TEST( AS5047P, SetZPOSLNonSettableBitsLow )
//{
//}
//
//IGNORE_TEST( AS5047P, SetSETTINGS1NonSettableBitsLow )
//{
//}
//
//IGNORE_TEST( AS5047P, SetSETTINGS2NonSettableBitsLow )
//{
//}

TEST( AS5047P, GetERRFLMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PERRFL errfl = { 0 };
    AS5047P_GetERRFL( &errfl );

    TEST_ASSERT_EQUAL( 0, errfl.parerr  );
    TEST_ASSERT_EQUAL( 1, errfl.invcomm );
    TEST_ASSERT_EQUAL( 0, errfl.frerr   );
}

TEST( AS5047P, GetPROGMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PPROG prog = { 0 };
    AS5047P_GetPROG( &prog );

    TEST_ASSERT_EQUAL( 1, prog.progver );
    TEST_ASSERT_EQUAL( 1, prog.progotp );
    TEST_ASSERT_EQUAL( 0, prog.otpref  );
    TEST_ASSERT_EQUAL( 0, prog.progen  );
}

TEST( AS5047P, GetDIAAGCMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PDIAAGC diaagc = { 0 };
    AS5047P_GetDIAAGC( &diaagc );

    TEST_ASSERT_EQUAL( 0, diaagc.magl );
    TEST_ASSERT_EQUAL( 0, diaagc.magh );
    TEST_ASSERT_EQUAL( 0, diaagc.cof );
    TEST_ASSERT_EQUAL( 1, diaagc.lf );
    TEST_ASSERT_EQUAL_HEX8( 0x5A, diaagc.agc );
}

TEST( AS5047P, GetMAGMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
    
    AS5047PMAG mag = { 0 };
    AS5047P_GetMAG( &mag );

    TEST_ASSERT_EQUAL_HEX16( 0x015A ,mag.mag );
}

TEST( AS5047P, GetANGLEUNCMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PANGLEUNC angleunc = { 0 };
    AS5047P_GetANGLEUNC( &angleunc );

    TEST_ASSERT_EQUAL_HEX16( 0x015A , angleunc.angleunc );
}

TEST( AS5047P, GetANGLECOMMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PANGLECOM anglecom = { 0 };
    AS5047P_GetANGLECOM( &anglecom );

    TEST_ASSERT_EQUAL_HEX16( 0x015A , anglecom.anglecom );
}

TEST( AS5047P, GetZPOSMMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PZPOSM zposm = { 0 };
    AS5047P_GetZPOSM( &zposm );

    TEST_ASSERT_EQUAL_HEX8( 0x5A, zposm.zposMSB );
}

TEST( AS5047P, GetZPOSLMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PZPOSL zposl = { 0 };
    AS5047P_GetZPOSL( &zposl );

    TEST_ASSERT_EQUAL( 26, zposl.zposLSB        );
    TEST_ASSERT_EQUAL( 1, zposl.comp_l_error_en );
    TEST_ASSERT_EQUAL( 0, zposl.comp_h_error_en );
}

TEST( AS5047P, GetSETTINGS1MemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PSETTINGS1 settings1 = { 0 };
    AS5047P_GetSETTINGS1( &settings1 );

    TEST_ASSERT_EQUAL( 0, settings1.factory_setting );
    TEST_ASSERT_EQUAL( 1, settings1.not_used        );
    TEST_ASSERT_EQUAL( 0, settings1.dir             );
    TEST_ASSERT_EQUAL( 1, settings1.uvw_abi         );
    TEST_ASSERT_EQUAL( 1, settings1.daecdis         );
    TEST_ASSERT_EQUAL( 0, settings1.abibin          );
    TEST_ASSERT_EQUAL( 1, settings1.dataselect      );
    TEST_ASSERT_EQUAL( 0, settings1.pwmOn           );
}

TEST( AS5047P, GetSETTINGS2MemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x80;
    nextReading[ 1 ] = 0x00;
    nextReading[ 2 ] = 0x81;
    nextReading[ 3 ] = 0x5A;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    AS5047PSETTINGS2 settings2 = { 0 };
    AS5047P_GetSETTINGS2( &settings2 );

    TEST_ASSERT_EQUAL( 2, settings2.uvwpp  );
    TEST_ASSERT_EQUAL( 3, settings2.hys    );
    TEST_ASSERT_EQUAL( 2, settings2.abires );
}

