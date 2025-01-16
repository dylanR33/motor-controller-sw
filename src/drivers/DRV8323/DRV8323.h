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

typedef struct
{
    void     ( *spiWrite ) ( uint16_t cmdOut );
    uint16_t ( *spiRead  ) ( uint16_t cmdOut );
} DRV8323Interface;

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
} DRV8323DriverCtrl_Config;

typedef struct
{
    uint8_t lock;
    uint8_t idrivep_hs;
    uint8_t idriven_hs;
} DRV8323GateDriveHS_Config;

typedef struct
{
    uint8_t cbc;
    uint8_t tdrive;
    uint8_t idrivep_ls;
    uint8_t idriven_ls;
} DRV8323GateDriveLS_Config;

typedef struct
{
    uint8_t tretry;
    uint8_t dead_time;
    uint8_t ocp_mode;
    uint8_t ocp_deg;
    uint8_t vds_lvl;
} DRV8323OCPCtrl_Config;

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
} DRV8323CSACtrl_Config;


DRV8323Status DRV8323_SetInterface( DRV8323Interface inter );

void DRV8323_Write( uint16_t data, uint8_t address );

uint16_t DRV8323_Read( uint8_t address );

void DRV8323_SetDriverCtrlReg( DRV8323DriverCtrl_Config* config );

void DRV8323_SetGateDriveHSReg( DRV8323GateDriveHS_Config* config );

void DRV8323_SetGateDriveLSReg( DRV8323GateDriveLS_Config* config );

void DRV8323_SetOCPCtrlReg( DRV8323OCPCtrl_Config* config );

void DRV8323_SetCSACtrlReg( DRV8323CSACtrl_Config* config );


#endif
