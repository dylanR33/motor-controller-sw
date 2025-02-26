#ifndef __DRV8323_H__
#define __DRV8323_H__

#include <stdint.h>


typedef enum
{
    DRV8323_INTERFACE_SET,
    DRV8323_INTERFACE_UNSET
} DRV8323Status;


enum
{
    OUT_OF_BOUNDS_ADDRESS
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
    ENABLE_OFF,
    ENABLE_ON
} DRV8323_EnableState;


typedef struct
{
    void     ( *spiWrite         ) ( uint8_t* data, uint16_t size );
    void     ( *spiRead          ) ( uint8_t* cmd,  uint8_t* rxBuff, uint16_t size );
    void     ( *spiSetCS         ) ( uint8_t state );
    uint32_t ( *adcReadRawPhaseA ) ( void );
    uint32_t ( *adcReadRawPhaseB ) ( void );
    uint32_t ( *adcReadRawPhaseC ) ( void );
    void     ( *setEnablePin     ) ( uint8_t state );
} DRV8323Interface;


typedef union
{
    struct
    {
        uint16_t vds_lc  : 1;
        uint16_t vds_hc  : 1;
        uint16_t vds_lb  : 1;
        uint16_t vds_hb  : 1;
        uint16_t vds_la  : 1;
        uint16_t vds_ha  : 1;
        uint16_t otsd    : 1;
        uint16_t uvlo    : 1;
        uint16_t gdf     : 1;
        uint16_t vds_ocp : 1;
        uint16_t fault   : 1;
        uint16_t _res    : 5;
    };
    uint16_t raw;
} DRV8323faultStatus1;


typedef union
{
    struct
    {
        uint16_t vgs_lc : 1;
        uint16_t vgs_hc : 1;
        uint16_t vgs_lb : 1;
        uint16_t vgs_hb : 1;
        uint16_t vgs_la : 1;
        uint16_t vgs_ha : 1;
        uint16_t cpuv   : 1;
        uint16_t otw    : 1;
        uint16_t sc_oc  : 1;
        uint16_t sb_oc  : 1;
        uint16_t sa_oc  : 1;
        uint16_t _res   : 5;
    };
    uint16_t raw;
} DRV8323faultStatus2;


typedef union
{
    struct
    {
        uint16_t clr_flt  : 1;
        uint16_t brake    : 1;
        uint16_t coast    : 1;
        uint16_t pwm1_dir : 1;
        uint16_t pwm1_com : 1;
        uint16_t pwm_mode : 2;
        uint16_t otw_rep  : 1;
        uint16_t dis_gdf  : 1;
        uint16_t dis_cpuv : 1;
        uint16_t _res     : 6;
    };
    uint16_t raw;
} DRV8323driverCtrl;


typedef union
{
    struct
    {
        uint16_t idriven_hs : 4;
        uint16_t idrivep_hs : 4;
        uint16_t lock       : 3;
        uint16_t _res       : 5;
    };
    uint16_t raw;
} DRV8323gateDriveHS;


typedef union
{
    struct
    {
        uint16_t idriven_ls : 4;
        uint16_t idrivep_ls : 4;
        uint16_t tdrive     : 2;
        uint16_t cbc        : 1;
        uint16_t _res       : 5;
    };
    uint16_t raw;
} DRV8323gateDriveLS;


typedef union
{
    struct
    {
        uint16_t vds_lvl   : 4;
        uint16_t ocp_deg   : 2;
        uint16_t ocp_mode  : 2;
        uint16_t dead_time : 2;
        uint16_t tretry    : 1;
        uint16_t _res      : 5;
    };
    uint16_t raw;
} DRV8323ocpCtrl;


typedef union
{
    struct
    {
        uint16_t sen_lvl   : 2;
        uint16_t csa_cal_c : 1;
        uint16_t csa_cal_b : 1;
        uint16_t csa_cal_a : 1;
        uint16_t dis_sen   : 1;
        uint16_t csa_gain  : 2;
        uint16_t ls_ref    : 1;
        uint16_t vref_div  : 1;
        uint16_t csa_fet   : 1;
        uint16_t _res      : 5;
    };
    uint16_t raw;
} DRV8323csaCtrl;


typedef struct
{
    DRV8323faultStatus1 faultStat1;
    DRV8323faultStatus2 faultStat2;
    DRV8323driverCtrl   driverCtrl;
    DRV8323gateDriveHS  gateDriveHS;
    DRV8323gateDriveLS  gateDriveLS;
    DRV8323ocpCtrl      ocpCtrl;
    DRV8323csaCtrl      csaCtrl;
} DRV8323Registers;


typedef struct
{
    float    vRef;
    uint32_t adcSteps;
    float    rSense;
    uint16_t csaGain;
} DRV8323CurrentSenseCfg;


DRV8323Status DRV8323_SetInterface( DRV8323Interface* inter );

void DRV8323_Write( uint16_t data, uint8_t address );

uint16_t DRV8323_Read( uint8_t address );

void DRV8323_SetDriverCtrl( DRV8323driverCtrl* config );

void DRV8323_SetGateDriveHS( DRV8323gateDriveHS* config );

void DRV8323_SetGateDriveLS( DRV8323gateDriveLS* config );

void DRV8323_SetOCPCtrl( DRV8323ocpCtrl* config );

void DRV8323_SetCSACtrl( DRV8323csaCtrl* config );

void DRV8323_GetFaultStatus1( DRV8323faultStatus1* fstat1 );

void DRV8323_GetFaultStatus2( DRV8323faultStatus2* fstat2 );

void DRV8323_GetDriverCtrl( DRV8323driverCtrl* driverCtrl );

void DRV8323_GetGateDriveHS( DRV8323gateDriveHS* gateDriveHS );

void DRV8323_GetGateDriveLS( DRV8323gateDriveLS* gateDriveLS );

void DRV8323_GetOCPCtrl( DRV8323ocpCtrl* ocpCtrl );

void DRV8323_GetCSACtrl( DRV8323csaCtrl* csaCtrl );

float DRV8323_GetPhaseCurrentA( DRV8323CurrentSenseCfg* iCfg );

float DRV8323_GetPhaseCurrentB( DRV8323CurrentSenseCfg* iCfg );

float DRV8323_GetPhaseCurrentC( DRV8323CurrentSenseCfg* iCfg );

void DRV8323_SetEnableState( DRV8323_EnableState state );

#endif
