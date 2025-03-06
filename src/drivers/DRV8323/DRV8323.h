#ifndef __DRV8323_H__
#define __DRV8323_H__

#include <stdint.h>
#include <assert.h>


typedef enum
{
    DRV8323_INTERFACE_SET,
    DRV8323_INTERFACE_UNSET
} DRV8323Status;


typedef enum
{
    OUT_OF_BOUNDS_ADDRESS
} DRV8323Failures;


// Register address definitions
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


// State of enable pin
typedef enum
{
    ENABLE_OFF,
    ENABLE_ON
} DRV8323_EnableState;


// Interface defining callbacks for SPI communication, ADC sampling, and GPIO logic levels
//
// spiWrite  : SPI transmission callback
//      data : buffer containing transmision data
//      size : size in bytes of transmission data buffer
//
// spiRead     : SPI transmission-receival callback
//      cmd    : buffer containing transmission data
//      rxBuff : buffer to store read back data
//      size   : size of both buffers (should be equal)
//
// spiSetCS   : SPI chip select gpio logic state setting callback
//      state : chip select gpio state (1 = high, 0 = low)
//
// adcReadRawPhaseA : ADC sampling callback
//
// adcReadRawPhaseB : ADC sampling callback
//
// adcReadRawPhaseC : ADC sampling callback
//
// setEnablePin : enable pin GPIO logic state setting callback
//      state : gpio state (1 = high, 0 = low)
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


// The following unions define the data significance contained within each register.
// The anonymous structures within define the bit layout of the structure through the
// use of bit fields. A raw value is provided within the union for easy access of the
// full register word.
//
// Checks for the size of the structure in bytes are included due to possible portability 
// issues with bit-fields across different compilers and target hardware.

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


static_assert( sizeof( DRV8323faultStatus1 ) ==  2, 
               "Size of DRV8323faultStatus1 struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323faultStatus2 ) ==  2, 
               "Size of DRV8323faultStatus2 struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323driverCtrl ) ==  2, 
               "Size of DRV8323driverCtrl struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323gateDriveHS ) ==  2, 
               "Size of DRV8323gateDriveHS struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323gateDriveLS ) ==  2, 
               "Size of DRV8323gateDriveLS struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323ocpCtrl ) ==  2, 
               "Size of DRV8323ocpCtrl struct incorrect expected 2 bytes." );

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


static_assert( sizeof( DRV8323csaCtrl ) ==  2, 
               "Size of DRV8323csaCtrl struct incorrect expected 2 bytes." );


// Structure used for calculation of phase current from ADC
// sample voltage.
//
// vref : ADC reference voltage
//
// adcSteps : max integer value producable by ADC (2^resolution)
//
// rSense : ohms sense resistor
//
// csaGain : current sense amplifier gain
typedef struct
{
    float    vRef;
    uint32_t adcSteps;
    float    rSense;
    uint16_t csaGain;
} DRV8323CurrentSenseCfg;


// Install hardware callbacks into the driver. All callbacks must
// be defined to be successful.
//
// inter : structure defining each callback function
//
// returns : status of interface setting
DRV8323Status DRV8323_SetInterface( DRV8323Interface* inter );


// Write data value into register.
//
// data : data value to write into register address
//
// address : address of register to write to
void DRV8323_Write( uint16_t data, uint8_t address );


// Read data of register.
//
// address : address of register to read from
//
// return : read back data
uint16_t DRV8323_Read( uint8_t address );


// Configure driver control register.
//
// config : union defining values of each field within the register
void DRV8323_SetDriverCtrl( DRV8323driverCtrl* config );


// Configure gate drive high side register.
//
// config : union defining values of each field within the register
void DRV8323_SetGateDriveHS( DRV8323gateDriveHS* config );


// Configure gate drive low side register.
//
// config : union defining values of each field within the register
void DRV8323_SetGateDriveLS( DRV8323gateDriveLS* config );


// Configure overcurrent protection register.
//
// config : union defining values of each field within the register
void DRV8323_SetOCPCtrl( DRV8323ocpCtrl* config );


// Configure current sense amplifier register.
//
// config : union defining values of each field within the register
void DRV8323_SetCSACtrl( DRV8323csaCtrl* config );


// Retreive fault status 1 register.
//
// fstat1 : union to hold each field within the register
void DRV8323_GetFaultStatus1( DRV8323faultStatus1* fstat1 );


// Retreive fault status 2 register.
//
// fstat2 : union to hold each field within the register
void DRV8323_GetFaultStatus2( DRV8323faultStatus2* fstat2 );


// Retreive driver control register.
//
// driverCtrl : union to hold each field within the register
void DRV8323_GetDriverCtrl( DRV8323driverCtrl* driverCtrl );


// Retreive gate drive high side register.
//
// gateDriveHS : union to hold each field within the register
void DRV8323_GetGateDriveHS( DRV8323gateDriveHS* gateDriveHS );


// Retreive gate drive low side register.
//
// gateDriveLS : union to hold each field within the register
void DRV8323_GetGateDriveLS( DRV8323gateDriveLS* gateDriveLS );


// Retreive over current protection register.
//
// ocpCtrl : union to hold each field within the register
void DRV8323_GetOCPCtrl( DRV8323ocpCtrl* ocpCtrl );


// Retreive current sense amplifier register.
//
// csaCtrl : union to hold each field within the register
void DRV8323_GetCSACtrl( DRV8323csaCtrl* csaCtrl );


// Retrive phase current of phase A
//
// iCfg : structure defining values used in calculation of phase current
//
// return : calculated phase current
float DRV8323_GetPhaseCurrentA( DRV8323CurrentSenseCfg* iCfg );


// Retrive phase current of phase B
//
// iCfg : structure defining values used in calculation of phase current
//
// return : calculated phase current
float DRV8323_GetPhaseCurrentB( DRV8323CurrentSenseCfg* iCfg );


// Retrive phase current of phase C
//
// iCfg : structure defining values used in calculation of phase current
//
// return : calculated phase current
float DRV8323_GetPhaseCurrentC( DRV8323CurrentSenseCfg* iCfg );


// Set the GPIO logic state of the enable pin
//
// state : enable logic state (1 = high, 0 = low)
void DRV8323_SetEnableState( DRV8323_EnableState state );

#endif
