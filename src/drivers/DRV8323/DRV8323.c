#include "DRV8323.h"

#include <stddef.h>


static DRV8323Interface interface = {NULL};


enum 
{
    RW_BIT              = 0x8000,
    ADDRESS_UPPER_BOUND = 0x06,
    ADDRESS_START_BIT   = 11,
    NON_DATA_BITS       = 0xF800,
};


enum
{
    SINGLE_BIT_MASK = 0x01,
    TWO_BIT_MASK    = 0x03,
    THREE_BIT_MASK  = 0x07,
    FOUR_BIT_MASK   = 0x0F
};


typedef enum
{
    VDS_LC_Pos,
    VDS_HC_Pos,
    VDS_LB_Pos,
    VDS_HB_Pos,
    VDS_LA_Pos,
    VDS_HA_Pos,
    OTSD_Pos,
    UVLO_Pos,
    GDF_Pos,
    VDS_OCP_Pos,
    FAULT_Pos,
} FaultStatus1_BitPos;


typedef enum
{
    VGS_LC_Pos,
    VGS_HC_Pos,
    VGS_LB_Pos,
    VGS_HB_Pos,
    VGS_LA_Pos,
    VGS_HA_Pos,
    CPUV_Pos,
    OTW_Pos,
    SC_OC_Pos,
    SB_OC_Pos,
    SA_OC_Pos,
} FaultStatus2_BitPos;


typedef enum
{
    CLR_FLT_Pos,
    BRAKE_Pos,
    COAST_Pos,
    PWM1_DIR_Pos,
    PWM1_COM_Pos,
    PWM_MODE_Pos,
    OTW_REP_Pos = 7,
    DIS_GDF_Pos,
    DIS_CPUV_Pos,
    RESERVED_Pos
} DriverCtrl_BitPos;


typedef enum
{
    IDRIVEN_HS_Pos,
    IDRIVEP_HS_Pos = 4,
    LOCK_Pos       = 8
} GateDriveHS_BitPos;


typedef enum
{
    IDRIVEN_LS_Pos,
    IDRIVEP_LS_Pos = 4,
    TDRIVE_Pos     = 8,
    CBC_Pos        = 10
} GateDriveLS_BitPos;


typedef enum
{
    VDS_LVL_Pos,
    OCP_DEG_Pos   = 4,
    OCP_MODE_Pos  = 6,
    DEAD_TIME_Pos = 8,
    TRETRY_Pos    = 10
} OCPCtrl_BitPos;


typedef enum
{
    SEN_LVL_Pos,
    CSA_CAL_C_Pos = 2,
    CSA_CAL_B_Pos,
    CSA_CAL_A_Pos,
    DIS_SEN_Pos,
    CSA_GAIN_Pos,
    LS_REF_Pos    = 8,
    VREF_DIV_Pos,
    CSA_FET_Pos
} CSACtrl_BitPos;
 

static int isAddressOutOfRange( uint8_t address )
{
    return address > ADDRESS_UPPER_BOUND;
}


static int isInterfaceValid( DRV8323Interface* inter )
{
    return ( inter->spiRead          && inter->spiWrite         && 
             inter->spiSetCS         && inter->adcReadRawPhaseA && 
             inter->adcReadRawPhaseB && inter->adcReadRawPhaseC && 
             inter->setEnablePin );
}


static int isAddressReadOnly( uint8_t address )
{
    return ( address == FAULT_STATUS1 || address == FAULT_STATUS2 );
}


static void clearNonDataBits( uint16_t* data )
{
    *data &= ~NON_DATA_BITS;
}


static void placeAddress( uint16_t* data, uint8_t address )
{
    *data |= ( address << ADDRESS_START_BIT );
}


static void clearReadWriteBit( uint16_t* data )
{
    *data &= ~RW_BIT;
}


static void setReadWriteBit( uint16_t* data )
{
    *data |= RW_BIT;
}

static void splitDataIntoBuffer( uint16_t data, uint8_t* buf )
{
    buf[0] = data >> 8;
    buf[1] = data;
}

static uint16_t extractDataFromBuffer( uint8_t* buf )
{
    return ( buf[0] << 8 ) | buf[1];
}


DRV8323Status DRV8323_SetInterface( DRV8323Interface* inter )
{
    if ( isInterfaceValid( inter ) )
    {
        interface = *inter;
        return DRV8323_INTERFACE_SET;
    }

    return DRV8323_INTERFACE_UNSET;
}


void DRV8323_Write( uint16_t data, uint8_t address )
{
    if ( isAddressOutOfRange( address ) )
        return;

    if ( isAddressReadOnly( address ) )
        return;

    clearNonDataBits( &data );

    placeAddress( &data, address );

    clearReadWriteBit( &data );

    uint8_t dataOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( data, dataOut );

    interface.spiSetCS( 0 );
    interface.spiWrite( dataOut, sizeof( dataOut) );
    interface.spiSetCS( 1 );
}


uint16_t DRV8323_Read( uint8_t address ) 
{
    if (  isAddressOutOfRange( address ) )
        return OUT_OF_BOUNDS_ADDRESS;

    uint16_t cmd = 0;

    placeAddress( &cmd, address );

    setReadWriteBit( &cmd );

    uint8_t cmdOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( cmd, cmdOut );
    uint8_t rxBuff[ sizeof( uint16_t ) ];

    interface.spiSetCS( 0 );
    interface.spiRead( cmdOut, rxBuff, sizeof( cmdOut ) );
    interface.spiSetCS( 1 );

    return extractDataFromBuffer( rxBuff );
}


static int isBitCountExceeded( uint8_t value, uint8_t bitCountMask )
{
    return ( value & ~bitCountMask );
}


void DRV8323_SetDriverCtrl( DRV8323DriverCtrl* config )
{
    // Check if bits in members exceed expected
    if  ( isBitCountExceeded( config->dis_cpuv, SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->dis_gdf , SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->otw_rep , SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->pwm_mode, TWO_BIT_MASK    ) ||
          isBitCountExceeded( config->pwm1_com, SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->pwm1_dir, SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->coast   , SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->brake   , SINGLE_BIT_MASK ) ||
          isBitCountExceeded( config->clr_flt , SINGLE_BIT_MASK )    )
        return;

    uint16_t dataOut = 0;

    dataOut |= config->clr_flt  << CLR_FLT_Pos 
            |  config->brake    << BRAKE_Pos
            |  config->coast    << COAST_Pos
            |  config->pwm1_dir << PWM1_DIR_Pos
            |  config->pwm1_com << PWM1_COM_Pos
            |  config->pwm_mode << PWM_MODE_Pos
            |  config->otw_rep  << OTW_REP_Pos
            |  config->dis_gdf  << DIS_GDF_Pos
            |  config->dis_cpuv << DIS_CPUV_Pos;
    
    // Clear reserved bit
    dataOut &= ~( 1 << RESERVED_Pos );

    DRV8323_Write( dataOut, DRIVER_CTRL );
}

void DRV8323_SetGateDriveHS( DRV8323GateDriveHS* config )
{
    if ( isBitCountExceeded( config->lock      , THREE_BIT_MASK ) ||
         isBitCountExceeded( config->idrivep_hs, FOUR_BIT_MASK  ) ||
         isBitCountExceeded( config->idriven_hs, FOUR_BIT_MASK  )   )
        return;

    uint16_t dataOut = 0;

    dataOut |= config->idriven_hs << IDRIVEN_HS_Pos
            |  config->idrivep_hs << IDRIVEP_HS_Pos
            |  config->lock       << LOCK_Pos;

    DRV8323_Write( dataOut, GATE_DRIVE_HS );
}


void DRV8323_SetGateDriveLS( DRV8323GateDriveLS* config )
{
    if ( isBitCountExceeded( config->cbc       , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->tdrive    , TWO_BIT_MASK    ) ||
         isBitCountExceeded( config->idrivep_ls, FOUR_BIT_MASK   ) ||
         isBitCountExceeded( config->idriven_ls, FOUR_BIT_MASK   )    )
        return;

    uint16_t dataOut = 0;

    dataOut |= config->idriven_ls << IDRIVEN_LS_Pos
            |  config->idrivep_ls << IDRIVEP_LS_Pos
            |  config->tdrive     << TDRIVE_Pos
            |  config->cbc        << CBC_Pos;

    DRV8323_Write( dataOut, GATE_DRIVE_LS );
}


void DRV8323_SetOCPCtrl( DRV8323OCPCtrl* config )
{
    if ( isBitCountExceeded( config->tretry   , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->dead_time, TWO_BIT_MASK    ) ||
         isBitCountExceeded( config->ocp_mode , TWO_BIT_MASK    ) ||
         isBitCountExceeded( config->ocp_deg  , TWO_BIT_MASK    ) ||
         isBitCountExceeded( config->vds_lvl  , FOUR_BIT_MASK   )   )
        return;

    uint16_t dataOut = 0;
    
    dataOut |= config->vds_lvl   << VDS_LVL_Pos
            |  config->ocp_deg   << OCP_DEG_Pos
            |  config->ocp_mode  << OCP_MODE_Pos
            |  config->dead_time << DEAD_TIME_Pos
            |  config->tretry    << TRETRY_Pos;

    DRV8323_Write( dataOut, OCP_CTRL );
}


void DRV8323_SetCSACtrl( DRV8323CSACtrl* config )
{
    if ( isBitCountExceeded( config->csa_fet  , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->vref_div , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->ls_ref   , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->csa_gain , TWO_BIT_MASK    ) ||
         isBitCountExceeded( config->dis_sen  , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->csa_cal_a, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->csa_cal_b, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->csa_cal_c, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->sen_lvl  , TWO_BIT_MASK    )   )
        return;

    uint16_t dataOut = 0;

    dataOut |= config->sen_lvl   << SEN_LVL_Pos
            |  config->csa_cal_c << CSA_CAL_C_Pos
            |  config->csa_cal_b << CSA_CAL_B_Pos
            |  config->csa_cal_a << CSA_CAL_A_Pos
            |  config->dis_sen   << DIS_SEN_Pos
            |  config->csa_gain  << CSA_GAIN_Pos
            |  config->ls_ref    << LS_REF_Pos
            |  config->vref_div  << VREF_DIV_Pos
            |  config->csa_fet   << CSA_FET_Pos;

    DRV8323_Write( dataOut, CSA_CTRL );
}


static uint8_t extractBits( uint16_t value, uint16_t mask, uint16_t bitPos )
{
    return ( ( value & ( mask << bitPos ) ) >> bitPos );
}


void DRV8323_GetFaultStatus1( DRV8323FaultStatus1* fstat1 )
{
    uint16_t reading = DRV8323_Read( FAULT_STATUS1 );

    fstat1->fault   = extractBits( reading, SINGLE_BIT_MASK, FAULT_Pos   );
    fstat1->vds_ocp = extractBits( reading, SINGLE_BIT_MASK, VDS_OCP_Pos );
    fstat1->gdf     = extractBits( reading, SINGLE_BIT_MASK, GDF_Pos     );
    fstat1->uvlo    = extractBits( reading, SINGLE_BIT_MASK, UVLO_Pos    );
    fstat1->otsd    = extractBits( reading, SINGLE_BIT_MASK, OTSD_Pos    );
    fstat1->vds_ha  = extractBits( reading, SINGLE_BIT_MASK, VDS_HA_Pos  );
    fstat1->vds_la  = extractBits( reading, SINGLE_BIT_MASK, VDS_LA_Pos  );
    fstat1->vds_hb  = extractBits( reading, SINGLE_BIT_MASK, VDS_HB_Pos  );
    fstat1->vds_lb  = extractBits( reading, SINGLE_BIT_MASK, VDS_LB_Pos  );
    fstat1->vds_hc  = extractBits( reading, SINGLE_BIT_MASK, VDS_HC_Pos  );
    fstat1->vds_lc  = extractBits( reading, SINGLE_BIT_MASK, VDS_LC_Pos  );
}


void DRV8323_GetFaultStatus2( DRV8323FaultStatus2* fstat2 )
{
    uint16_t reading = DRV8323_Read( FAULT_STATUS2 );

    fstat2->sa_oc  = extractBits( reading, SINGLE_BIT_MASK, SA_OC_Pos   );
    fstat2->sb_oc  = extractBits( reading, SINGLE_BIT_MASK, SB_OC_Pos   );
    fstat2->sc_oc  = extractBits( reading, SINGLE_BIT_MASK, SC_OC_Pos   );
    fstat2->otw    = extractBits( reading, SINGLE_BIT_MASK, OTW_Pos     );
    fstat2->cpuv   = extractBits( reading, SINGLE_BIT_MASK, CPUV_Pos    );
    fstat2->vgs_ha = extractBits( reading, SINGLE_BIT_MASK, VGS_HA_Pos  );
    fstat2->vgs_la = extractBits( reading, SINGLE_BIT_MASK, VGS_LA_Pos  );
    fstat2->vgs_hb = extractBits( reading, SINGLE_BIT_MASK, VGS_HB_Pos  );
    fstat2->vgs_lb = extractBits( reading, SINGLE_BIT_MASK, VGS_LB_Pos  );
    fstat2->vgs_hc = extractBits( reading, SINGLE_BIT_MASK, VGS_HC_Pos  );
    fstat2->vgs_lc = extractBits( reading, SINGLE_BIT_MASK, VGS_LC_Pos  );
}


void DRV8323_GetDriverCtrl( DRV8323DriverCtrl* driverCtrl )
{
    uint16_t reading = DRV8323_Read( DRIVER_CTRL );

    driverCtrl->dis_cpuv = extractBits( reading, SINGLE_BIT_MASK, DIS_CPUV_Pos );
    driverCtrl->dis_gdf  = extractBits( reading, SINGLE_BIT_MASK, DIS_GDF_Pos  );
    driverCtrl->otw_rep  = extractBits( reading, SINGLE_BIT_MASK, OTW_Pos      );
    driverCtrl->pwm_mode = extractBits( reading, TWO_BIT_MASK   , PWM_MODE_Pos );
    driverCtrl->pwm1_com = extractBits( reading, SINGLE_BIT_MASK, PWM1_COM_Pos );
    driverCtrl->coast    = extractBits( reading, SINGLE_BIT_MASK, COAST_Pos    );
    driverCtrl->brake    = extractBits( reading, SINGLE_BIT_MASK, BRAKE_Pos    );
    driverCtrl->clr_flt  = extractBits( reading, SINGLE_BIT_MASK, CLR_FLT_Pos  );
}


void DRV8323_GetGateDriveHS( DRV8323GateDriveHS* gateDriveHS )
{
    uint16_t reading = DRV8323_Read( GATE_DRIVE_HS );

    gateDriveHS->lock       = extractBits( reading, THREE_BIT_MASK, LOCK_Pos       );
    gateDriveHS->idrivep_hs = extractBits( reading, FOUR_BIT_MASK , IDRIVEP_HS_Pos );
    gateDriveHS->idriven_hs = extractBits( reading, FOUR_BIT_MASK , IDRIVEN_HS_Pos );
}

void DRV8323_GetGateDriveLS( DRV8323GateDriveLS* gateDriveLS )
{
    uint16_t reading = DRV8323_Read( GATE_DRIVE_LS );

    gateDriveLS->cbc        = extractBits( reading, SINGLE_BIT_MASK, CBC_Pos        );
    gateDriveLS->tdrive     = extractBits( reading, TWO_BIT_MASK   , TDRIVE_Pos     );
    gateDriveLS->idrivep_ls = extractBits( reading, FOUR_BIT_MASK  , IDRIVEP_LS_Pos );
    gateDriveLS->idriven_ls = extractBits( reading, FOUR_BIT_MASK  , IDRIVEN_LS_Pos );
}


void DRV8323_GetOCPCtrl( DRV8323OCPCtrl* ocpCtrl )
{
    uint16_t reading = DRV8323_Read( OCP_CTRL );
    
    ocpCtrl->tretry    = extractBits( reading, SINGLE_BIT_MASK, TRETRY_Pos    );
    ocpCtrl->dead_time = extractBits( reading, TWO_BIT_MASK   , DEAD_TIME_Pos );
    ocpCtrl->ocp_mode  = extractBits( reading, TWO_BIT_MASK   , OCP_MODE_Pos  );
    ocpCtrl->ocp_deg   = extractBits( reading, TWO_BIT_MASK   , OCP_DEG_Pos   );
    ocpCtrl->vds_lvl   = extractBits( reading, FOUR_BIT_MASK  , VDS_LVL_Pos   );
}


void DRV8323_GetCSACtrl( DRV8323CSACtrl* csaCtrl )
{
    uint16_t reading = DRV8323_Read( CSA_CTRL );

    csaCtrl->csa_fet   = extractBits( reading, SINGLE_BIT_MASK, CSA_FET_Pos   );
    csaCtrl->vref_div  = extractBits( reading, SINGLE_BIT_MASK, VREF_DIV_Pos  );
    csaCtrl->ls_ref    = extractBits( reading, SINGLE_BIT_MASK, LS_REF_Pos    );
    csaCtrl->csa_gain  = extractBits( reading, TWO_BIT_MASK   , CSA_GAIN_Pos  );
    csaCtrl->dis_sen   = extractBits( reading, SINGLE_BIT_MASK, DIS_SEN_Pos   );
    csaCtrl->csa_cal_a = extractBits( reading, SINGLE_BIT_MASK, CSA_CAL_A_Pos );
    csaCtrl->csa_cal_b = extractBits( reading, SINGLE_BIT_MASK, CSA_CAL_B_Pos );
    csaCtrl->csa_cal_c = extractBits( reading, SINGLE_BIT_MASK, CSA_CAL_C_Pos );
    csaCtrl->sen_lvl   = extractBits( reading, TWO_BIT_MASK   , SEN_LVL_Pos   );
}


static float convertAdcToVoltage( uint32_t rawAdc, DRV8323CurrentSenseCfg* iCfg )
{
    return ( ( iCfg->vRef / iCfg->adcSteps ) * rawAdc );
}


static float calculatePhaseCurrent( float vSox, DRV8323CurrentSenseCfg* iCfg )
{
    return ( ( iCfg->vRef / 2 ) - vSox ) / ( iCfg->csaGain * iCfg->rSense );
}


float DRV8323_GetPhaseCurrentA( DRV8323CurrentSenseCfg* iCfg )
{
    return calculatePhaseCurrent( convertAdcToVoltage( interface.adcReadRawPhaseA(), iCfg ), iCfg );
}


float DRV8323_GetPhaseCurrentB( DRV8323CurrentSenseCfg* iCfg )
{
    return calculatePhaseCurrent( convertAdcToVoltage( interface.adcReadRawPhaseB(), iCfg ), iCfg );
}


float DRV8323_GetPhaseCurrentC( DRV8323CurrentSenseCfg* iCfg )
{
    return calculatePhaseCurrent( convertAdcToVoltage( interface.adcReadRawPhaseC(), iCfg ), iCfg );
}

void DRV8323_SetEnableState( DRV8323_EnableState state )
{
    if ( state == ENABLE_ON )
        interface.setEnablePin( 1 );
    else
        interface.setEnablePin( 0 );
}
