#include "DRV8323.h"

#include "unity.h"
#include "unity_fixture.h"

#include <stdint.h>


#include "WriteSpy.h"
#include "FakeRead.h"


TEST_GROUP( DRV8323 );


uint8_t nextReading[ sizeof( uint16_t ) ] = { 0, 0};

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

    WriteSpy_Create();

    FakeRead_Create();
    nextReading[ 0 ] = 0;
    nextReading[ 1 ] = 0;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );
}

TEST_TEAR_DOWN( DRV8323 )
{
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

TEST( DRV8323, PreventReadToOutOfBoundsAddresses )
{
    TEST_ASSERT_EQUAL( OUT_OF_BOUNDS_ADDRESS, DRV8323_Read( 0x08 ) );
    TEST_ASSERT_NULL( FakeRead_GetLastCmd8Arr() );
}

TEST( DRV8323, PreventWriteToReadOnlyAddresses )
{
    DRV8323_Write( 0xFFFF, 0x01 );
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_BITS( 0x07FF, 0x02B5, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x07FF, 0x039A, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x07FF, 0x015F, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x07FF, 0x046C, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_BITS( 0x07FF, 0x5A6, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
}

TEST( DRV8323, SetGateDriveHSFullFrameCheck )
{
    DRV8323GateDriveHS config = 
    {
        .lock = 3,
        .idrivep_hs = 9,
        .idriven_hs = 10
    };
    DRV8323_SetGateDriveHS( &config );
    TEST_ASSERT_EQUAL_HEX32( 0x1B9A, concatenate16( WriteSpy_GetLastWrite8Arr() ) );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
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
    TEST_ASSERT_NULL( WriteSpy_GetLastWrite8Arr() );
}

TEST( DRV8323, GetFaultStatus1MemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

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
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

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
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

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
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    DRV8323GateDriveHS gateDriveHS = { 0 };
    DRV8323_GetGateDriveHS( &gateDriveHS );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.lock       );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.idrivep_hs );
    TEST_ASSERT_EQUAL( 5, gateDriveHS.idriven_hs );
}

TEST( DRV8323, GetGateDriveLSMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

    DRV8323GateDriveLS gateDriveLS = { 0 };
    DRV8323_GetGateDriveLS( &gateDriveLS );
    TEST_ASSERT_EQUAL( 1, gateDriveLS.cbc        );
    TEST_ASSERT_EQUAL( 1, gateDriveLS.tdrive     );
    TEST_ASSERT_EQUAL( 5, gateDriveLS.idrivep_ls );
    TEST_ASSERT_EQUAL( 5, gateDriveLS.idriven_ls );
}

TEST( DRV8323, GetOCPCtrlMemberBitsExtractedCorrectly )
{
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

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
    nextReading[ 0 ] = 0x05;
    nextReading[ 1 ] = 0x55;
    FakeRead_SetNextReading8Arr( nextReading, sizeof( nextReading ) );

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
