/*****************************************************************************/
/**
*
* @file robot_encoder.h
*
*
******************************************************************************/

#ifndef ROBOT_ENCODER_H
#define ROBOT_ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
 
 /* Debug Printf. */
#include "robot_debug.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"
 
/* Robot includes */
#include "robot_status.h"
 
/************************** Constant Definitions *****************************/

/**
  * @brief  SPI Interface pins
  */
#define SPIx                       SPI2
#define SPIx_CLK                   RCC_APB1Periph_SPI2

#define SPIx_SCK_PIN               GPIO_Pin_13                  /* PB.13 */
#define SPIx_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define SPIx_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE            GPIO_PinSource13
#define SPIx_SCK_AF                GPIO_AF_SPI2

#define SPIx_MISO_PIN              GPIO_Pin_14                  /* PB.14 */
#define SPIx_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define SPIx_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPIx_MISO_SOURCE           GPIO_PinSource14
#define SPIx_MISO_AF               GPIO_AF_SPI1

#define SPIx_MOSI_PIN              GPIO_Pin_15                  /* PB.15 */
#define SPIx_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define SPIx_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE           GPIO_PinSource15
#define SPIx_MOSI_AF               GPIO_AF_SPI1

#define SPIx_CS_PIN                GPIO_Pin_12                  /* PB.12 */
#define SPIx_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define SPIx_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

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

void EncoderInitialise(void);
void EncoderRefershTask( void *pvParameters );
void SPISelfTest(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_ENCODER_H */
