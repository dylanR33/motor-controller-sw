#include "DRV8323.h"

#include <stddef.h>


static DRV8323Interface interface = {NULL};


enum 
{
    ADDRESS_UPPER_BOUND = 0x06,
};


typedef enum
{
    CS_OFF,
    CS_ON
} CSState;


typedef union
{
    struct
    {
        uint16_t data : 11;
        uint16_t addr : 4;
        uint16_t rw   : 1;
    };
    uint16_t raw;
} CommandFrame;


typedef union
{
    struct
    {
        uint16_t data : 11;
        uint16_t _res : 5;
    };
    uint16_t raw;
} ReadFrame;


static int isInterfaceValid( DRV8323Interface* inter )
{
    return ( inter->spiRead          && inter->spiWrite         && 
             inter->spiSetCS         && inter->adcReadRawPhaseA && 
             inter->adcReadRawPhaseB && inter->adcReadRawPhaseC && 
             inter->setEnablePin );
}


static int isAddressOutOfRange( uint8_t address )
{
    return address > ADDRESS_UPPER_BOUND;
}


static int isAddressReadOnly( uint8_t address )
{
    return ( address == FAULT_STATUS1 || address == FAULT_STATUS2 );
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

    CommandFrame cmd = { 0 };
    cmd.addr = address;
    cmd.data = data;

    uint8_t cmdOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( cmd.raw, cmdOut );

    interface.spiSetCS( CS_OFF );
    interface.spiWrite( cmdOut, sizeof( cmdOut) );
    interface.spiSetCS( CS_ON );
}


uint16_t DRV8323_Read( uint8_t address ) 
{
    if (  isAddressOutOfRange( address ) )
        return OUT_OF_BOUNDS_ADDRESS;

    CommandFrame cmd = { 0 };
    cmd.addr = address;
    cmd.rw = 1;

    uint8_t cmdOut[ sizeof( uint16_t ) ];
    splitDataIntoBuffer( cmd.raw, cmdOut );

    uint8_t rxBuff[ sizeof( uint16_t ) ];

    interface.spiSetCS( CS_OFF );
    interface.spiRead( cmdOut, rxBuff, sizeof( cmdOut ) );
    interface.spiSetCS( CS_ON );
    
    ReadFrame rx = { 0 };
    rx.raw = extractDataFromBuffer( rxBuff );

    return rx.data;
}


void DRV8323_SetDriverCtrl( DRV8323driverCtrl* config )
{
    DRV8323_Write( config->raw, DRIVER_CTRL );
}


void DRV8323_SetGateDriveHS( DRV8323gateDriveHS* config )
{
    DRV8323_Write( config->raw, GATE_DRIVE_HS );
}


void DRV8323_SetGateDriveLS( DRV8323gateDriveLS* config )
{
    DRV8323_Write( config->raw, GATE_DRIVE_LS );
}


void DRV8323_SetOCPCtrl( DRV8323ocpCtrl* config )
{
    DRV8323_Write( config->raw, OCP_CTRL );
}


void DRV8323_SetCSACtrl( DRV8323csaCtrl* config )
{
    DRV8323_Write( config->raw, CSA_CTRL );
}


void DRV8323_GetFaultStatus1( DRV8323faultStatus1* fstat1 )
{
    fstat1->raw = DRV8323_Read( FAULT_STATUS1 );
}


void DRV8323_GetFaultStatus2( DRV8323faultStatus2* fstat2 )
{
    fstat2->raw = DRV8323_Read( FAULT_STATUS2 );
}


void DRV8323_GetDriverCtrl( DRV8323driverCtrl* driverCtrl )
{
    driverCtrl->raw = DRV8323_Read( DRIVER_CTRL );
}


void DRV8323_GetGateDriveHS( DRV8323gateDriveHS* gateDriveHS )
{
    gateDriveHS->raw = DRV8323_Read( GATE_DRIVE_HS );
}

void DRV8323_GetGateDriveLS( DRV8323gateDriveLS* gateDriveLS )
{
    gateDriveLS->raw = DRV8323_Read( GATE_DRIVE_LS );
}


void DRV8323_GetOCPCtrl( DRV8323ocpCtrl* ocpCtrl )
{
    ocpCtrl->raw = DRV8323_Read( OCP_CTRL );
}


void DRV8323_GetCSACtrl( DRV8323csaCtrl* csaCtrl )
{
    csaCtrl->raw = DRV8323_Read( CSA_CTRL );
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
        interface.setEnablePin( ENABLE_ON );
    else
        interface.setEnablePin( ENABLE_OFF );
}

