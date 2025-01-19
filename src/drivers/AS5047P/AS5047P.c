#include "AS5047P.h"

#include <stddef.h>


static AS5047PInterface interface = { NULL };


enum
{
    SINGLE_BIT_MASK   = 0x01,
    TWO_BIT_MASK      = 0x03,
    THREE_BIT_MASK    = 0x07,
    SIX_BIT_MASK      = 0x3F,
    EIGHT_BIT_MASK    = 0xFF,
    FOURTEEN_BIT_MASK = 0x3FFF
};


typedef enum
{
    COMMAND_FRAME_MASK  = 0xFFFF0000,
    READ_WRITE_BIT_MASK = 0x40000000,
    PARC_MASK           = 0x80000000,
    PARD_MASK           = 0x00008000,
    EF_MASK             = 0x00004000,
} FrameMasks;


typedef enum
{
    ADDR_Pos = 16
} Frame_BitPos;


typedef enum
{
    FRERR_Pos,
    INVCOMM_Pos,
    PARERR_Pos
} ERRFL_BitPos;


typedef enum
{
    PROGEN_Pos,
    OTPREF_Pos  = 2,
    PROGOTP_Pos,
    PROGVER_Pos = 6
} PROG_BitPos;


typedef enum
{
    AGC_Pos,
    LF_Pos   = 8,
    COF_Pos,
    MAGH_Pos,
    MAGL_Pos
} DIAAGC_BitPos;


typedef enum
{
    MAG_Pos
} MAG_BitPos;


typedef enum
{
    ANGLEUNC_Pos
} ANGLEUNC_BitPos;


typedef enum
{
    ANGLECOM_Pos
} ANGLECOM_BitPos;


typedef enum
{
    ZPOSM_Pos
} ZPOSM_BitPos;


typedef enum
{
    ZPOSL_Pos,
    COMP_L_ERROR_EN_Pos = 6,
    COMP_H_ERROR_EN_Pos
} ZPOSL_BitPos;


typedef enum
{
    FACTORY_SETTINGS_Pos,
    NOT_USED_Pos,
    DIR_Pos,
    UVW_ABI_Pos,
    DAECDIS_Pos,
    ABIBIN_Pos,
    DATASELECT_Pos,
    PWM_ON_Pos
} SETTINGS1_BitPos;


typedef enum
{
    UVWPP_Pos,
    HYS_Pos    = 3,
    ABIRES_Pos = 5
} SETTINGS2_BitPos;
 

AS5047PStatus AS5047P_SetInterface( AS5047PInterface inter )
{
    if ( inter.spiWrite && inter.spiRead )
    {
        interface.spiWrite = inter.spiWrite;
        interface.spiRead  = inter.spiRead;

        return AS5047P_INTERFACE_SET;
    }

    return AS5047P_INTERFACE_UNSET;
}


static int isAddressReadOnly( uint16_t address )
{
    return ( ( address < ZPOSM || address > SETTINGS2 ) && address != PROG );
}


static void clearCmdFrame( uint32_t* data )
{
    *data &= ~( COMMAND_FRAME_MASK );
}


static void placeAddress( uint32_t* data, uint16_t address )
{
    // remove extra bits off address
    address &= ~( 0xC000 );
    // Place address at bit 16
    *data |= address << ADDR_Pos;
}


static void clearReadWriteBit( uint32_t* data )
{
    *data &= ~( READ_WRITE_BIT_MASK );
}


static void setReadWriteBit( uint32_t* data )
{
    *data |= READ_WRITE_BIT_MASK;
}


static void clearParityAndEFBits( uint32_t* data )
{
    *data &= ~( PARC_MASK | PARD_MASK | EF_MASK );
}


static int isAddressOutOfBounds( uint16_t address )
{
    return ( ( address < ZPOSM   ||   address >  SETTINGS2 ) &&
             ( address < DIAAGC  ||   address >  ANGLECOM  ) &&
             ( address > ERRFL ) && ( address != PROG      )    );
}


static int areParityBitsSet( uint32_t reading )
{
    return ( reading & PARC_MASK ) && ( reading & PARD_MASK );
}


static int isEFBitSet( uint32_t reading )
{
    return reading & EF_MASK;
}


void AS5047P_Write( uint32_t data, uint16_t address )
{
    // return if read only address
    if ( isAddressReadOnly( address ) )
        return;

    // Clear command frame portion
    clearCmdFrame( &data );
    
    placeAddress( &data, address );

    // Set r/w bit low
    clearReadWriteBit( &data ); 

    // Set parity and EF bits low
    clearParityAndEFBits( &data );

    interface.spiWrite( data );
}


uint32_t AS5047P_Read( uint16_t address )
{
    if ( isAddressOutOfBounds( address ) )
        return AS5047P_OUT_OF_BOUNDS_ADDRESS;

    uint32_t cmdOut = 0;

    placeAddress( &cmdOut, address );
    
    // Set r/w bit high
    setReadWriteBit( &cmdOut );

    // Set parity and EF bits low
    clearParityAndEFBits( &cmdOut );

    uint32_t readingBack = interface.spiRead( cmdOut );

    // Check if parity bits back are set, if not fail
    if ( !areParityBitsSet( readingBack ) )
        return AS5047P_PARITY_FAIL;
    
    // Check if EF bit is set, if so fail
    if ( isEFBitSet( readingBack ) )
        return AS5047P_EF_FAIL;

    return readingBack;
}


static int isBitCountExceeded( uint16_t value, uint16_t bitCountMask )
{
    return ( value & ~bitCountMask );
}


void AS5047P_SetPROG( AS5047PPROG* config )
{

    // Check if bits in members exceed expected
    if ( isBitCountExceeded( config->progver, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->progotp, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->otpref , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->progen , SINGLE_BIT_MASK )   )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->progen  << PROGEN_Pos
            |  config->otpref  << OTPREF_Pos
            |  config->progotp << PROGOTP_Pos
            |  config->progver << PROGVER_Pos;

    // Clear non-settable bits
    //dataOut &= ~( 0x00000000 );

    AS5047P_Write( dataOut, PROG );
}


void AS5047P_SetZPOSM( AS5047PZPOSM* config )
{
    // No need to check if bits exceeded since
    // zposm is 8 bits

    uint32_t dataOut = 0;

    dataOut |= config->zposMSB;

    AS5047P_Write( dataOut, ZPOSM );
}


void AS5047P_SetZPOSL( AS5047PZPOSL* config )
{
    // Check if bits in members exceed expected
    if ( isBitCountExceeded( config->zposLSB        , SIX_BIT_MASK    ) ||
         isBitCountExceeded( config->comp_l_error_en, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->comp_h_error_en, SINGLE_BIT_MASK )   )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->zposLSB         << ZPOSL_Pos
            |  config->comp_l_error_en << COMP_L_ERROR_EN_Pos
            |  config->comp_h_error_en << COMP_H_ERROR_EN_Pos;

    AS5047P_Write( dataOut, ZPOSL );
}


void AS5047P_SetSETTINGS1( AS5047PSETTINGS1* config )
{
    if ( isBitCountExceeded( config->dir       , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->uvw_abi   , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->daecdis   , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->abibin    , SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->dataselect, SINGLE_BIT_MASK ) ||
         isBitCountExceeded( config->pwmOn     , SINGLE_BIT_MASK )   )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->dir        << DIR_Pos
            |  config->uvw_abi    << UVW_ABI_Pos
            |  config->daecdis    << DAECDIS_Pos
            |  config->abibin     << ABIBIN_Pos
            |  config->dataselect << DATASELECT_Pos
            |  config->pwmOn      << PWM_ON_Pos;

    AS5047P_Write( dataOut, SETTINGS1 );
}


void AS5047P_SetSETTINGS2( AS5047PSETTINGS2* config )
{
    if ( isBitCountExceeded( config->uvwpp , THREE_BIT_MASK ) ||
         isBitCountExceeded( config->hys   , TWO_BIT_MASK   ) ||
         isBitCountExceeded( config->abires, THREE_BIT_MASK )   )
        return;

    uint32_t dataOut = 0;

    dataOut |= config->uvwpp  << UVWPP_Pos
            |  config->hys    << HYS_Pos
            |  config->abires << ABIRES_Pos;

    AS5047P_Write( dataOut, SETTINGS2 );
}


static uint16_t extractBits( uint32_t value, uint32_t mask, uint32_t bitPos )
{
    return ( ( value & ( mask << bitPos ) ) >> bitPos );
}


void AS5047P_GetERRFL( AS5047PERRFL* errfl )
{
    uint32_t reading = AS5047P_Read( ERRFL );

    errfl->parerr  = extractBits( reading, SINGLE_BIT_MASK, PARERR_Pos  );
    errfl->invcomm = extractBits( reading, SINGLE_BIT_MASK, INVCOMM_Pos );
    errfl->frerr   = extractBits( reading, SINGLE_BIT_MASK, FRERR_Pos   );
}


void AS5047P_GetPROG( AS5047PPROG* prog )
{
    uint32_t reading = AS5047P_Read( PROG );

    prog->progver = extractBits( reading, SINGLE_BIT_MASK, PROGVER_Pos );
    prog->progotp = extractBits( reading, SINGLE_BIT_MASK, PROGOTP_Pos );
    prog->otpref  = extractBits( reading, SINGLE_BIT_MASK, OTPREF_Pos  );
    prog->progen  = extractBits( reading, SINGLE_BIT_MASK, PROGEN_Pos  );
}


void AS5047P_GetDIAAGC( AS5047PDIAAGC* diaagc )
{
    uint32_t reading = AS5047P_Read( DIAAGC );

    diaagc->magl = extractBits( reading, SINGLE_BIT_MASK, MAGL_Pos );
    diaagc->magh = extractBits( reading, SINGLE_BIT_MASK, MAGH_Pos );
    diaagc->cof  = extractBits( reading, SINGLE_BIT_MASK, COF_Pos  );
    diaagc->lf   = extractBits( reading, SINGLE_BIT_MASK, LF_Pos   );
    diaagc->agc  = extractBits( reading, EIGHT_BIT_MASK , AGC_Pos  );
}


void AS5047P_GetMAG( AS5047PMAG* mag )
{
    uint32_t reading = AS5047P_Read( MAG );
    
    mag->mag = extractBits( reading, FOURTEEN_BIT_MASK, MAG_Pos );
}


void AS5047P_GetANGLEUNC( AS5047PANGLEUNC* angleunc )
{
    uint32_t reading = AS5047P_Read( ANGLEUNC );

    angleunc->angleunc = extractBits( reading, FOURTEEN_BIT_MASK, ANGLEUNC_Pos );
}


void AS5047P_GetANGLECOM( AS5047PANGLECOM* anglecom )
{
    uint32_t reading = AS5047P_Read( ANGLECOM );

    anglecom->anglecom = extractBits( reading, FOURTEEN_BIT_MASK, ANGLECOM_Pos );
}


void AS5047P_GetZPOSM( AS5047PZPOSM* zposm )
{
    uint32_t reading = AS5047P_Read( ZPOSM );

    zposm->zposMSB = extractBits( reading, EIGHT_BIT_MASK, ZPOSM_Pos );
}


void AS5047P_GetZPOSL( AS5047PZPOSL* zposl )
{
    uint32_t reading = AS5047P_Read( ZPOSL );

    zposl->zposLSB         = extractBits( reading, SIX_BIT_MASK   , ZPOSL_Pos           );
    zposl->comp_l_error_en = extractBits( reading, SINGLE_BIT_MASK, COMP_L_ERROR_EN_Pos );
    zposl->comp_h_error_en = extractBits( reading, SINGLE_BIT_MASK, COMP_H_ERROR_EN_Pos );
}


void AS5047P_GetSETTINGS1( AS5047PSETTINGS1* settings1 )
{
    uint32_t reading = AS5047P_Read( SETTINGS1 );

    settings1->factory_setting = extractBits( reading, SINGLE_BIT_MASK, FACTORY_SETTINGS_Pos );
    settings1->not_used        = extractBits( reading, SINGLE_BIT_MASK, NOT_USED_Pos         );
    settings1->dir             = extractBits( reading, SINGLE_BIT_MASK, DIR_Pos              );
    settings1->uvw_abi         = extractBits( reading, SINGLE_BIT_MASK, UVW_ABI_Pos          );
    settings1->daecdis         = extractBits( reading, SINGLE_BIT_MASK, DAECDIS_Pos          );
    settings1->abibin          = extractBits( reading, SINGLE_BIT_MASK, ABIBIN_Pos           );
    settings1->dataselect      = extractBits( reading, SINGLE_BIT_MASK, DATASELECT_Pos       );
    settings1->pwmOn           = extractBits( reading, SINGLE_BIT_MASK, PWM_ON_Pos           );
}


void AS5047P_GetSETTINGS2( AS5047PSETTINGS2* settings2 )
{
    uint32_t reading = AS5047P_Read( SETTINGS2 );
    
    settings2->uvwpp  = extractBits( reading, THREE_BIT_MASK, UVWPP_Pos  );
    settings2->hys    = extractBits( reading, TWO_BIT_MASK  , HYS_Pos    );
    settings2->abires = extractBits( reading, THREE_BIT_MASK, ABIRES_Pos );
}
