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

#define SPIx_DMA                   	DMA1
#define SPIx_DMA_CLK               	RCC_AHB1Periph_DMA1
#define SPIx_TX_DMA_CHANNEL        	DMA_Channel_0
#define SPIx_TX_DMA_STREAM         	DMA1_Stream4
#define SPIx_TX_DMA_FLAG_TCIF      	DMA_FLAG_TCIF4
#define SPIx_RX_DMA_CHANNEL        	DMA_Channel_0
#define DMA_STREAM_IRQ           	DMA1_Stream3_IRQn
#define SPIx_RX_DMA_STREAM         	DMA1_Stream3
#define SPIx_RX_DMA_FLAG_TCIF      	DMA_FLAG_TCIF3
#define DMA_STREAM_IRQHANDLER    	DMA1_Stream3_IRQHandler
#define SPIx_DMA_BUFFER_SIZE       	(0x15*3)
 
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
