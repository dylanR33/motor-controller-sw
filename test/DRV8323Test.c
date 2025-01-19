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
        .spiWrite = WriteSpy_Write16,
        .spiRead = FakeRead_Read16
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
        .spiWrite = WriteSpy_Write16,
        .spiRead = NULL
    };

    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_UNSET, DRV8323_SetInterface( interface ) );
    
    interface.spiRead = FakeRead_Read16;

    TEST_ASSERT_EQUAL( DRV8323_INTERFACE_SET, DRV8323_SetInterface( interface ) );
}

TEST( DRV8323, WriteMSBLow )
{
    DRV8323_Write( 0xFFFF, 0x03 );
    TEST_ASSERT_BIT_LOW( 15, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, ReadMSBHigh )
{
    DRV8323_Read( 0x01 );
    TEST_ASSERT_BIT_HIGH( 15 ,FakeRead_GetLastCmd16() );
}

TEST( DRV8323, PreventWriteToOutOfBoundsAddresses )
{
    DRV8323_Write( 0xFFFF, 0x07 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, PreventReadToOutOfBoundsAddresses )
{
    TEST_ASSERT_EQUAL( OUT_OF_BOUNDS_ADDRESS, DRV8323_Read( 0x08 ) );
    TEST_ASSERT_EQUAL( 0, FakeRead_GetLastCmd16() );
}

TEST( DRV8323, PreventWriteToReadOnlyAddresses )
{
    DRV8323_Write( 0xFFFF, 0x01 );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, RestrictDataToBits10Through0 )
{
    DRV8323_Write( 0xFFFF, 0x02 );
    TEST_ASSERT_EQUAL_HEX16( 0x17FF, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, ReadDataBitsCleared )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS_LOW( 0x07FF, FakeRead_GetLastCmd16() );
}

TEST( DRV8323, WritePlaceAddressInBits14Through11 )
{
    DRV8323_Write( 0xFFFF, 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, ReadPlaceAddressInBits14Through11 )
{
    DRV8323_Read( 0x05 );
    TEST_ASSERT_BITS( 0x7800, 0x2800, FakeRead_GetLastCmd16() );
}

TEST( DRV8323, SetDriverCtrlMemberBitsPlacedCorrectly )
{
    DRV8323DriverCtrl config =
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

    DRV8323_SetDriverCtrl( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x02B5, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetGateDriveHSMemberBitsPlacedCorrectly )
{
    DRV8323GateDriveHS config = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };
    DRV8323_SetGateDriveHS( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x039A, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetGateDriveLSMemberBitsPlacedCorrectly )
{
    DRV8323GateDriveLS config = 
    {
        .cbc = 0,
        .tdrive = 1,
        .idrivep_ls = 5,
        .idriven_ls = 15
    };
    DRV8323_SetGateDriveLS( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x015F, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetOCPCtrlMemberBitsPlacedCorrectly )
{
    DRV8323OCPCtrl config = 
    {
        .tretry = 1,
        .dead_time = 0,
        .ocp_mode = 1,
        .ocp_deg = 2,
        .vds_lvl = 12
    };
    DRV8323_SetOCPCtrl( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x046C, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetCSACtrlMemberBitsPlacedCorrectly )
{
    DRV8323CSACtrl config = 
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
    DRV8323_SetCSACtrl( &config );
    TEST_ASSERT_BITS( 0x07FF, 0x5A6, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetDriverCtrlFailIfMembersBitsExceedExpected )
{
    DRV8323DriverCtrl config =
    {
        .dis_cpuv = 2,
        .dis_gdf = 0,
        .otw_rep = 1,
        .pwm_mode = 4,
        .pwm1_com = 1,
        .pwm1_dir = 3,
        .coast = 1,
        .brake = 0,
        .clr_flt = 1
    };

    DRV8323_SetDriverCtrl( &config );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetGateDriveHSFailIfMembersBitsExceedExpected )
{
    DRV8323GateDriveHS config = 
    {
        .lock = 9,
        .idrivep_hs = 16,
        .idriven_hs = 10
    };
    DRV8323_SetGateDriveHS( &config );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetGateDriveLSFailIfMembersBitsExceedExpected )
{
    DRV8323GateDriveLS config = 
    {
        .cbc = 2,
        .tdrive = 1,
        .idrivep_ls = 5,
        .idriven_ls = 16
    };
    DRV8323_SetGateDriveLS( &config );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetOCPCtrlFailIfMembersBitsExceedExpected )
{
    DRV8323OCPCtrl config = 
    {
        .tretry = 0,
        .dead_time = 0,
        .ocp_mode = 8,
        .ocp_deg = 2,
        .vds_lvl = 16
    };
    DRV8323_SetOCPCtrl( &config );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, SetCSACtrlFailIfMembersBitsExceedExpected )
{
    DRV8323CSACtrl config = 
    {
        .csa_fet = 2,
        .vref_div = 0,
        .ls_ref = 1,
        .csa_gain = 2,
        .dis_sen = 1,
        .csa_cal_a = 0,
        .csa_cal_b = 0,
        .csa_cal_c = 1,
        .sen_lvl = 2
    };
    DRV8323_SetCSACtrl( &config );
    TEST_ASSERT_EQUAL( 0, WriteSpy_GetLastWrite16() );
}

TEST( DRV8323, GetFaultStatus1MemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323FaultStatus1 fstat1 = { 0 };
    DRV8323_GetFaultStatus1( &fstat1 );
    TEST_ASSERT_EQUAL( 1, fstat1.fault   );
    TEST_ASSERT_EQUAL( 0, fstat1.vds_ocp );
    TEST_ASSERT_EQUAL( 1, fstat1.gdf     );
    TEST_ASSERT_EQUAL( 0, fstat1.uvlo    );
    TEST_ASSERT_EQUAL( 1, fstat1.otsd    );
    TEST_ASSERT_EQUAL( 0, fstat1.vds_ha  );
    TEST_ASSERT_EQUAL( 1, fstat1.vds_la  );
    TEST_ASSERT_EQUAL( 0, fstat1.vds_hb  );
    TEST_ASSERT_EQUAL( 1, fstat1.vds_lb  );
    TEST_ASSERT_EQUAL( 0, fstat1.vds_hc  );
    TEST_ASSERT_EQUAL( 1, fstat1.vds_lc  );
}

TEST( DRV8323, GetFaultStatus2MemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323FaultStatus2 fstat2 = { 0 };
    DRV8323_GetFaultStatus2( &fstat2 );
    TEST_ASSERT_EQUAL( 1, fstat2.sa_oc  );
    TEST_ASSERT_EQUAL( 0, fstat2.sb_oc  );
    TEST_ASSERT_EQUAL( 1, fstat2.sc_oc  );
    TEST_ASSERT_EQUAL( 0, fstat2.otw    );
    TEST_ASSERT_EQUAL( 1, fstat2.cpuv   );
    TEST_ASSERT_EQUAL( 0, fstat2.vgs_ha );
    TEST_ASSERT_EQUAL( 1, fstat2.vgs_la );
    TEST_ASSERT_EQUAL( 0, fstat2.vgs_hb );
    TEST_ASSERT_EQUAL( 1, fstat2.vgs_lb );
    TEST_ASSERT_EQUAL( 0, fstat2.vgs_hc );
    TEST_ASSERT_EQUAL( 1, fstat2.vgs_lc );
}

TEST( DRV8323, GetDriverCtrlMemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323DriverCtrl driverCtrl = { 0 };
    DRV8323_GetDriverCtrl( &driverCtrl );
    TEST_ASSERT_EQUAL( 0, driverCtrl.dis_cpuv );
    TEST_ASSERT_EQUAL( 1, driverCtrl.dis_gdf  );
    TEST_ASSERT_EQUAL( 0, driverCtrl.otw_rep  );
    TEST_ASSERT_EQUAL( 2, driverCtrl.pwm_mode );
    TEST_ASSERT_EQUAL( 1, driverCtrl.pwm1_com );
    TEST_ASSERT_EQUAL( 0, driverCtrl.pwm1_dir );
    TEST_ASSERT_EQUAL( 1, driverCtrl.coast    );
    TEST_ASSERT_EQUAL( 0, driverCtrl.brake    );
    TEST_ASSERT_EQUAL( 1, driverCtrl.clr_flt  );
}

TEST( DRV8323, GetGateDriveHSMemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323GateDriveHS gateDriveHS = { 0 };
    DRV8323_GetGateDriveHS( &gateDriveHS );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.lock       );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.idrivep_hs );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.idriven_hs );
}

TEST( DRV8323, GetGateDriveLSMemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323GateDriveLS gateDriveLS = { 0 };
    DRV8323_GetGateDriveLS( &gateDriveLS );
    TEST_ASSERT_EQUAL( 1, gateDriveLS.cbc        );
    TEST_ASSERT_EQUAL( 1, gateDriveLS.tdrive     );
    TEST_ASSERT_EQUAL( 5, gateDriveLS.idrivep_ls );
    TEST_ASSERT_EQUAL( 5, gateDriveLS.idriven_ls );
}

TEST( DRV8323, GetOCPCtrlMemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323OCPCtrl ocpCtrl = { 0 };
    DRV8323_GetOCPCtrl( &ocpCtrl );
    TEST_ASSERT_EQUAL( 1, ocpCtrl.tretry    );
    TEST_ASSERT_EQUAL( 1, ocpCtrl.dead_time );
    TEST_ASSERT_EQUAL( 1, ocpCtrl.ocp_mode  );
    TEST_ASSERT_EQUAL( 1, ocpCtrl.ocp_deg   );
    TEST_ASSERT_EQUAL( 5, ocpCtrl.vds_lvl   );
}

TEST( DRV8323, GetCSACtrlMemberBitsExtractedCorrectly )
{
    FakeRead_SetNextReading16( 0x0555 );

    DRV8323CSACtrl csaCtrl = { 0 };
    DRV8323_GetCSACtrl( &csaCtrl );
    TEST_ASSERT_EQUAL( 1, csaCtrl.csa_fet   );
    TEST_ASSERT_EQUAL( 0, csaCtrl.vref_div  );
    TEST_ASSERT_EQUAL( 1, csaCtrl.ls_ref    );
    TEST_ASSERT_EQUAL( 1, csaCtrl.csa_gain  );
    TEST_ASSERT_EQUAL( 0, csaCtrl.dis_sen   );
    TEST_ASSERT_EQUAL( 1, csaCtrl.csa_cal_a );
    TEST_ASSERT_EQUAL( 0, csaCtrl.csa_cal_b );
    TEST_ASSERT_EQUAL( 1, csaCtrl.csa_cal_c );
    TEST_ASSERT_EQUAL( 1, csaCtrl.sen_lvl   );
}

