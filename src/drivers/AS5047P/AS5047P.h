#ifndef __AS5047P_H__
#define __AS5047P_H__

#include <stdint.h>


typedef enum
{
    AS5047P_INTERFACE_SET,
    AS5047P_INTERFACE_UNSET
} AS5047PStatus;


typedef enum
{
    AS5047P_OUT_OF_BOUNDS_ADDRESS,
    AS5047P_PARITY_FAIL,
    AS5047P_EF_FAIL
} AS5047PFailures;

// Register address values
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


// Structure defining callbacks to SPI interaction functions.
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
//      state : chip select gpio state ( 1 = high, 0 = low )
typedef struct
{
    void ( *spiWrite )( uint8_t* data, uint16_t size );
    void ( *spiRead  )( uint8_t* cmd, uint8_t* rxBuff, uint16_t size );
    void ( *spiSetCS )( uint8_t  state );
} AS5047PInterface;


// The following unions define the data significance contained within each register.
// The anonymous structures within define the bit layout of the structure through the
// use of bit fields. A raw value is provided within the union for easy access of the
// full register word.

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


// Install the SPI callbacks into the driver. All callbacks
// must be defined to be successful.
//
// inter : structure defining each SPI callback function
//
// returns : status of interface setting
AS5047PStatus AS5047P_SetInterface( AS5047PInterface* inter );


// Write data value into register.
//
// data : data value to write into register address
//
// address : address of register to write to
void AS5047P_Write( uint16_t data, uint16_t address);


// Read data of register.
//
// address : address of register to read from
//
// return : read back data
uint16_t AS5047P_Read( uint16_t address );


// Configure programming register.
//
// config : union defining values of each field within the register
void AS5047P_SetPROG( AS5047Pprog* config );


// Configure zero position MSB register.
//
// config : union defining values of each field within the register
void AS5047P_SetZPOSM( AS5047Pzposm* config );


// Configure zero position LSB / MAG register.
//
// config : union defining values of each field within the register
void AS5047P_SetZPOSL( AS5047Pzposl* config );


// Configure custom setting register 1.
//
// config : union defining values of each field within the register
void AS5047P_SetSETTINGS1( AS5047Psettings1* config );


// Configure custom setting register 2.
//
// config : union defining values of each field within the register
void AS5047P_SetSETTINGS2( AS5047Psettings2* config );


// Retreive error register.
//
// errfl : union to hold each field within the register
void AS5047P_GetERRFL( AS5047Perrfl* errfl );


// Retreive programming register.
//
// prog : union to hold each field within the register
void AS5047P_GetPROG( AS5047Pprog* prog );


// Retreive diagnostic / AGC register.
//
// diaagc : union to hold each field within the register
void AS5047P_GetDIAAGC( AS5047Pdiaagc* diaagc );


// Retreive CORDIC magnitude register.
//
// mag : union to hold each field within the register
void AS5047P_GetMAG( AS5047Pmag* mag );


// Retreive measured angle without dynamic angle error compensation register.
//
// angleunc : union to hold each field within the register
void AS5047P_GetANGLEUNC( AS5047Pangleunc* angleunc );


// Retreive measured angle with dynamic angle error compensation register.
//
// anglecom : union to hold each field within the register
void AS5047P_GetANGLECOM( AS5047Panglecom* anglecom );


// Retreive zero position MSB register.
//
// zposm : union to hold each field within the register
void AS5047P_GetZPOSM( AS5047Pzposm* zposm );


// Retreive zero position LSB / MAG register.
//
// zposl : union to hold each field within the register
void AS5047P_GetZPOSL( AS5047Pzposl* zposl );


// Retreive setting register 1.
//
// settings1 : union to hold each field within the register
void AS5047P_GetSETTINGS1( AS5047Psettings1* settings1 );


// Retreive setting register 2.
//
// settings2 : union to hold each field within the register
void AS5047P_GetSETTINGS2( AS5047Psettings2* settings2 );


#endif
