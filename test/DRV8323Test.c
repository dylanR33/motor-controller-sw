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

TEST( DRV8323, DriverCtrlMemberBitsPlacedCorrectly )
{
    DRV8323DriverCtrl_Config config =
    {
        .dis_cpuv = 1,
        .dis_gdf = 0,
        .otw_rep = 1,
        .pwm_mode = 1,
        .pwm1_com = 1,
        .pwm1_dir = 0,
        .coast = 1,
        .brake = 0,
        .clr_flt = 1
    };

    DRV8323_SetDriverCtrlReg( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x02B5, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, GateDriveHSMemberBitsPlacedCorrectly )
{
    DRV8323GateDriveHS_Config config = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };
    DRV8323_SetGateDriveHSReg( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x039A, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, GateDriveLSMemberBitsPlacedCorrectly )
{
    DRV8323GateDriveLS_Config config = 
    {
        .cbc = 0,
        .tdrive = 1,
        .idrivep_ls = 5,
        .idriven_ls = 15
    };
    DRV8323_SetGateDriveLSReg( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x015F, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, OCPCtrlMemberBitsPlacedCorrectly )
{
    DRV8323OCPCtrl_Config config = 
    {
        .tretry = 1,
        .dead_time = 0,
        .ocp_mode = 1,
        .ocp_deg = 2,
        .vds_lvl = 12
    };
    DRV8323_SetOCPCtrlReg( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x046C, WriteSpy_GetLastWrite() );
}

TEST( DRV8323, CSACtrlMemberBitsPlacedCorrectly )
{
    DRV8323CSACtrl_Config config = 
    {
        .csa_fet = 1,
        .vref_div = 0,
        .ls_ref = 1,
        .csa_gain = 2,
        .dis_sen = 1,
        .csa_cal_a = 0,
        .csa_cal_b = 0,
        .csa_cal_c = 1,
        .sen_lvl = 2
    };
    DRV8323_SetCSACtrlReg( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x5A6, WriteSpy_GetLastWrite() );
}
