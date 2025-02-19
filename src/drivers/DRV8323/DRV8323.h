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
    void     ( *spiWrite         ) ( uint16_t cmdOut );
    uint16_t ( *spiRead          ) ( uint16_t cmdOut );
    uint32_t ( *adcReadRawPhaseA ) ( void            );
    uint32_t ( *adcReadRawPhaseB ) ( void            );
    uint32_t ( *adcReadRawPhaseC ) ( void            );
    void     ( *setEnablePin     ) ( uint8_t state   );
} DRV8323Interface;

typedef struct
{
    uint8_t fault;
    uint8_t vds_ocp;
    uint8_t gdf;
    uint8_t uvlo;
    uint8_t otsd;
    uint8_t vds_ha;
    uint8_t vds_la;
    uint8_t vds_hb;
    uint8_t vds_lb;
    uint8_t vds_hc;
    uint8_t vds_lc;
} DRV8323FaultStatus1;

typedef struct
{
    uint8_t sa_oc;
    uint8_t sb_oc;
    uint8_t sc_oc;
    uint8_t otw;
    uint8_t cpuv;
    uint8_t vgs_ha;
    uint8_t vgs_la;
    uint8_t vgs_hb;
    uint8_t vgs_lb;
    uint8_t vgs_hc;
    uint8_t vgs_lc;
} DRV8323FaultStatus2;

typedef struct
{
    uint8_t dis_cpuv;
    uint8_t dis_gdf;
    uint8_t otw_rep;
    uint8_t pwm_mode;
    uint8_t pwm1_com;
    uint8_t pwm1_dir;
    uint8_t coast;
    uint8_t brake;
    uint8_t clr_flt;
} DRV8323DriverCtrl;

typedef struct
{
    uint8_t lock;
    uint8_t idrivep_hs;
    uint8_t idriven_hs;
} DRV8323GateDriveHS;

typedef struct
{
    uint8_t cbc;
    uint8_t tdrive;
    uint8_t idrivep_ls;
    uint8_t idriven_ls;
} DRV8323GateDriveLS;

typedef struct
{
    uint8_t tretry;
    uint8_t dead_time;
    uint8_t ocp_mode;
    uint8_t ocp_deg;
    uint8_t vds_lvl;
} DRV8323OCPCtrl;

typedef struct
{
    uint8_t csa_fet;
    uint8_t vref_div;
    uint8_t ls_ref;
    uint8_t csa_gain;
    uint8_t dis_sen;
    uint8_t csa_cal_a;
    uint8_t csa_cal_b;
    uint8_t csa_cal_c;
    uint8_t sen_lvl;
} DRV8323CSACtrl;

typedef struct
{
    DRV8323FaultStatus1 faultStat1;
    DRV8323FaultStatus2 faultStat2;
    DRV8323DriverCtrl   driverCtrl;
    DRV8323GateDriveHS  gateDriveHS;
    DRV8323GateDriveLS  gateDriveLS;
    DRV8323OCPCtrl      ocpCtrl;
    DRV8323CSACtrl      csaCtrl;
} DRV8323Registers;

typedef struct
{
    float vRef;
    uint32_t adcSteps;
    float rSense;
    uint16_t csaGain;
} DRV8323CurrentSenseCfg;

DRV8323Status DRV8323_SetInterface( DRV8323Interface* inter );

void DRV8323_Write( uint16_t data, uint8_t address );

uint16_t DRV8323_Read( uint8_t address );

void DRV8323_SetDriverCtrl( DRV8323DriverCtrl* config );

void DRV8323_SetGateDriveHS( DRV8323GateDriveHS* config );

void DRV8323_SetGateDriveLS( DRV8323GateDriveLS* config );

void DRV8323_SetOCPCtrl( DRV8323OCPCtrl* config );

void DRV8323_SetCSACtrl( DRV8323CSACtrl* config );

void DRV8323_GetFaultStatus1( DRV8323FaultStatus1* fstat1 );

void DRV8323_GetFaultStatus2( DRV8323FaultStatus2* fstat2 );

void DRV8323_GetDriverCtrl( DRV8323DriverCtrl* driverCtrl );

void DRV8323_GetGateDriveHS( DRV8323GateDriveHS* gateDriveHS );

void DRV8323_GetGateDriveLS( DRV8323GateDriveLS* gateDriveLS );

void DRV8323_GetOCPCtrl( DRV8323OCPCtrl* ocpCtrl );

void DRV8323_GetCSACtrl( DRV8323CSACtrl* csaCtrl );

float DRV8323_GetPhaseCurrentA( DRV8323CurrentSenseCfg* iCfg );

float DRV8323_GetPhaseCurrentB( DRV8323CurrentSenseCfg* iCfg );

float DRV8323_GetPhaseCurrentC( DRV8323CurrentSenseCfg* iCfg );

void DRV8323_SetEnableState( DRV8323_EnableState state );

#endif
