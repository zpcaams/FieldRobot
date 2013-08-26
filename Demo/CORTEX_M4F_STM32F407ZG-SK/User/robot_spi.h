/*****************************************************************************/
/**
*
* @file robot_spi.h
*
*
******************************************************************************/

#ifndef ROBOT_SPI_H
#define ROBOT_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"

/************************** Constant Definitions *****************************/

#define Encoder_TASK_PRIORITY		( tskIDLE_PRIORITY + 3UL )

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void GetSpiBuffer(u8 *pBuffer);
void SpiRefershTask( void *pvParameters );
void SPISelfTest(void);
void SpiInitialise(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_SPI_H */
