#ifndef __DRV8323_H__
#define __DRV8323_H__

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

#endif
