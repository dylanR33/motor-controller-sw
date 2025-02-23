#ifndef __AS5047P_H__
#define __AS5047P_H__

#include <stdint.h>


typedef enum
{
    AS5047P_INTERFACE_SET,
    AS5047P_INTERFACE_UNSET
} AS5047PStatus;


enum
{
    AS5047P_OUT_OF_BOUNDS_ADDRESS,
    AS5047P_PARITY_FAIL,
    AS5047P_EF_FAIL
};


typedef enum
{
    NOP,
    ERRFL,
    PROG      = 0x0003,
    DIAAGC    = 0x3FFC,
    MAG,
    ANGLEUNC,
    ANGLECOM,
    ZPOSM     = 0x0016,
    ZPOSL,
    SETTINGS1,
    SETTINGS2
} AS5047PRegister;


typedef struct
{
    void ( *spiWrite )( uint8_t* data, uint16_t size );
    void ( *spiRead  )( uint8_t* cmd, uint8_t* rxBuff, uint16_t size );
    void ( *spiSetCS )( uint8_t  state );
} AS5047PInterface;


typedef struct
{
    uint8_t parerr;
    uint8_t invcomm;
    uint8_t frerr;
} AS5047PERRFL;


typedef struct
{
    uint8_t progver;
    uint8_t progotp;
    uint8_t otpref;
    uint8_t progen;
} AS5047PPROG;


typedef struct
{
    uint8_t magl;
    uint8_t magh;
    uint8_t cof;
    uint8_t lf;
    uint8_t agc;
} AS5047PDIAAGC;


typedef struct
{
    uint16_t mag;
} AS5047PMAG;


typedef struct
{
    uint16_t angleunc;
} AS5047PANGLEUNC;


typedef struct
{
    uint16_t anglecom;
} AS5047PANGLECOM;


typedef struct
{
    uint8_t zposMSB;
} AS5047PZPOSM;


typedef struct
{
    uint8_t zposLSB;
    uint8_t comp_l_error_en;
    uint8_t comp_h_error_en;
} AS5047PZPOSL;


typedef struct
{
    uint8_t factory_setting;
    uint8_t not_used;
    uint8_t dir;
    uint8_t uvw_abi;
    uint8_t daecdis;
    uint8_t abibin;
    uint8_t dataselect;
    uint8_t pwmOn;
} AS5047PSETTINGS1;


typedef struct
{
    uint8_t uvwpp;
    uint8_t hys;
    uint8_t abires;
} AS5047PSETTINGS2;


AS5047PStatus AS5047P_SetInterface( AS5047PInterface* inter );

void AS5047P_Write( uint32_t data, uint16_t address );

uint32_t AS5047P_Read( uint16_t address );

void AS5047P_SetPROG( AS5047PPROG* config );

void AS5047P_SetZPOSM( AS5047PZPOSM* config );

void AS5047P_SetZPOSL( AS5047PZPOSL* config );

void AS5047P_SetSETTINGS1( AS5047PSETTINGS1* config );

void AS5047P_SetSETTINGS2( AS5047PSETTINGS2* config );

void AS5047P_GetERRFL( AS5047PERRFL* errfl );

void AS5047P_GetPROG( AS5047PPROG* prog );

void AS5047P_GetDIAAGC( AS5047PDIAAGC* diaagc );

void AS5047P_GetMAG( AS5047PMAG* mag );

void AS5047P_GetANGLEUNC( AS5047PANGLEUNC* angleunc );

void AS5047P_GetANGLECOM( AS5047PANGLECOM* anglecom );

void AS5047P_GetZPOSM( AS5047PZPOSM* zposm );

void AS5047P_GetZPOSL( AS5047PZPOSL* zposl );

void AS5047P_GetSETTINGS1( AS5047PSETTINGS1* settings1 );

void AS5047P_GetSETTINGS2( AS5047PSETTINGS2* settings2 );

#endif
