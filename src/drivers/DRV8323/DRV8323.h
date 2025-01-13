#ifndef __DRV8323_H__
#define __DRV8323_H__

#include <stdint.h>

// Want a simple typedef to a callback function for writing
// and reading over spi. Minimally it should just contain
// the following parameters:
//      data
//      data size
//      register
// Could Optionally be a typedef struct for Initialization DRV8323Interface 
// which gets filled by some initialization function DRV8323_SetInterface(DRV8323Interface interface)
//
// There will be a definition to this callback in the user code
// such that this callback internally calls HAL_SPI_XXX for
// transmission and receival. It will also take care of the
// parameters of 
//      SPI_HandleTypeDef *hspi
//      uint16_t timeout
//

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

DRV8323Status DRV8323_SetInterface( DRV8323Interface inter );

void DRV8323_Write( uint16_t data, uint8_t address );

uint16_t DRV8323_Read( uint8_t address );


#endif
