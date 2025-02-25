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


typedef union
{
    struct
    {
        uint16_t frerr   : 1;
        uint16_t invcomm : 1;
        uint16_t parerr  : 1;
        uint16_t _res    : 13;
    };
    uint16_t raw;
} AS5047Perrfl;


typedef union
{
    struct
    {
        uint16_t progen  : 1;
        uint16_t _res1   : 1;
        uint16_t otpref  : 1;
        uint16_t progotp : 1;
        uint16_t _res2   : 2;
        uint16_t progver : 1;
        uint16_t _res3   : 9;
    };
    uint16_t raw;
} AS5047Pprog;


typedef union
{
    struct
    {
        uint16_t agc  : 8;
        uint16_t lf   : 1;
        uint16_t cof  : 1;
        uint16_t magh : 1;
        uint16_t magl : 1;
        uint16_t _res : 4;
    };
    uint16_t raw;
} AS5047Pdiaagc;


typedef union
{
    struct
    {
        uint16_t mag  : 14;
        uint16_t _res : 2;
    };
    uint16_t raw;
} AS5047Pmag;


typedef union
{
    struct
    {
        uint16_t angleunc : 14;
        uint16_t _res     : 2;
    };
    uint16_t raw;
} AS5047Pangleunc;


typedef union
{
    struct
    {
        uint16_t anglecom : 14;
        uint16_t _res     : 2;
    };
    uint16_t raw;
} AS5047Panglecom;


typedef union
{
    struct
    {
        uint16_t zposm : 8;
        uint16_t _res  : 8;
    };
    uint16_t raw;
} AS5047Pzposm;


typedef union
{
    struct
    {
        uint16_t zposl           : 6;
        uint16_t comp_l_error_en : 1;
        uint16_t comp_h_error_en : 1;
        uint16_t _res            : 8;
    };
    uint16_t raw;
} AS5047Pzposl;


typedef union
{
    struct
    {
        uint16_t factory_setting : 1;
        uint16_t not_used        : 1;
        uint16_t dir             : 1;
        uint16_t uvw_abi         : 1;
        uint16_t daecdis         : 1;
        uint16_t abibin          : 1;
        uint16_t dataselect      : 1;
        uint16_t pwmOn           : 1;
        uint16_t _res            : 8;
    };
    uint16_t raw;
} AS5047Psettings1;


typedef union
{
    struct
    {
        uint16_t uvwpp  : 3;
        uint16_t hys    : 2;
        uint16_t abires : 3;
        uint16_t _res   : 8;
    };
    uint16_t raw;
} AS5047Psettings2;


AS5047PStatus AS5047P_SetInterface( AS5047PInterface* inter );

void AS5047P_Write( uint16_t data, uint16_t address);

uint16_t AS5047P_Read( uint16_t address );

void AS5047P_SetPROG( AS5047Pprog* config );

void AS5047P_SetZPOSM( AS5047Pzposm* config );

void AS5047P_SetZPOSL( AS5047Pzposl* config );

void AS5047P_SetSETTINGS1( AS5047Psettings1* config );

void AS5047P_SetSETTINGS2( AS5047Psettings2* config );

void AS5047P_GetERRFL( AS5047Perrfl* errfl );

void AS5047P_GetPROG( AS5047Pprog* prog );

void AS5047P_GetDIAAGC( AS5047Pdiaagc* diaagc );

void AS5047P_GetMAG( AS5047Pmag* mag );

void AS5047P_GetANGLEUNC( AS5047Pangleunc* angleunc );

void AS5047P_GetANGLECOM( AS5047Panglecom* anglecom );

void AS5047P_GetZPOSM( AS5047Pzposm* zposm );

void AS5047P_GetZPOSL( AS5047Pzposl* zposl );

void AS5047P_GetSETTINGS1( AS5047Psettings1* settings1 );

void AS5047P_GetSETTINGS2( AS5047Psettings2* settings2 );

#endif
