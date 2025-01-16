#include "DRV8323.h"

#include <stdio.h>

static DRV8323Interface interface = {NULL};

enum 
{
    FAULT_STATUS_REG,
    VSG_STAUS_REG,
    RW_BIT = 0x8000,
    ADDRESS_UPPER_BOUND = 0x06,
    ADDRESS_START_BIT = 11,
    NON_DATA_BITS = 0xF800
};

typedef enum
{
    FAULT_STATUS1,
    FAULT_STATUS2,
    DRIVER_CTRL,
    GATE_DRIVE_HS,
    GATE_DRIVE_LS,
    OCP_CTRL,
    CSA_CTRL
} DRV8323_Register;

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
    DIS_CPUV_Pos
} DriverCtrl_BitPos;

typedef enum
{
    IDRIVEN_HS_Pos,
    IDRIVEP_HS_Pos = 4,
    LOCK_Pos = 8
} GateDriveHS_BitPos;

typedef enum
{
    IDRIVEN_LS_Pos,
    IDRIVEP_LS_Pos = 4,
    TDRIVE_Pos = 8,
    CBC_Pos = 10
} GateDriveLS_BitPos;

typedef enum
{
    VDS_LVL_Pos,
    OCP_DEG_Pos = 4,
    OCP_MODE_Pos = 6,
    DEAD_TIME_Pos = 8,
    TRETRY_Pos = 10
} OCPCtrl_BitPos;

typedef enum
{
    SEN_LVL_Pos,
    CSA_CAL_C_Pos = 2,
    CSA_CAL_B_Pos,
    CSA_CAL_A_Pos,
    DIS_SEN_Pos,
    CSA_GAIN_Pos,
    LS_REF_Pos = 8,
    VREF_DIV_Pos,
    CSA_FET_Pos
} CSACtrl_BitPos;
 
static int isAddressOutOfRange( uint8_t address )
{
    return address > ADDRESS_UPPER_BOUND;
}

static int isInterfaceValid( DRV8323Interface inter )
{
    return ( inter.spiRead && inter.spiWrite );
}

static int isAddressReadOnly( uint8_t address )
{
    return ( address == FAULT_STATUS_REG || address == VSG_STAUS_REG );
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

DRV8323Status DRV8323_SetInterface( DRV8323Interface inter )
{
    if ( isInterfaceValid( inter ) )
    {
        interface = inter;
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

    interface.spiWrite( data );
}


uint16_t DRV8323_Read( uint8_t address ) 
{
    if (  isAddressOutOfRange( address ) )
        return OUT_OF_BOUNDS_ADDRESS;

    uint16_t cmdOut = 0;

    placeAddress( &cmdOut, address );

    setReadWriteBit( &cmdOut );

    return interface.spiRead( cmdOut );
}

void DRV8323_SetDriverCtrlReg( DRV8323DriverCtrl_Config* config )
{
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
    dataOut &= ~0x0400;

    DRV8323_Write( dataOut, DRIVER_CTRL );
}

void DRV8323_SetGateDriveHSReg( DRV8323GateDriveHS_Config* config )
{
    uint16_t dataOut = 0;

    dataOut |= config->idriven_hs << IDRIVEN_HS_Pos
            |  config->idrivep_hs << IDRIVEP_HS_Pos
            |  config->lock       << LOCK_Pos;

    DRV8323_Write( dataOut, GATE_DRIVE_HS );
}


void DRV8323_SetGateDriveLSReg( DRV8323GateDriveLS_Config* config )
{
    uint16_t dataOut = 0;

    dataOut |= config->idriven_ls << IDRIVEN_LS_Pos
            |  config->idrivep_ls << IDRIVEP_LS_Pos
            |  config->tdrive     << TDRIVE_Pos
            |  config->cbc        << CBC_Pos;

    DRV8323_Write( dataOut, GATE_DRIVE_LS );
}


void DRV8323_SetOCPCtrlReg( DRV8323OCPCtrl_Config* config )
{
    uint16_t dataOut = 0;
    
    dataOut |= config->vds_lvl   << VDS_LVL_Pos
            |  config->ocp_deg   << OCP_DEG_Pos
            |  config->ocp_mode  << OCP_MODE_Pos
            |  config->dead_time << DEAD_TIME_Pos
            |  config->tretry    << TRETRY_Pos;

    DRV8323_Write( dataOut, OCP_CTRL );
}


void DRV8323_SetCSACtrlReg( DRV8323CSACtrl_Config* config )
{
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
