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
 
 /* Debug Printf. */
#include "robot_debug.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"
   	 
/************************** Constant Definitions *****************************/


/* Uncomment the following line to use the default Encoder_TIMEOUT_UserCallback() 
   function implemented in stm32f4_discovery_Encoder.c file.
   Encoder_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */   
#define USE_DEFAULT_TIMEOUT_CALLBACK 

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPIx_FLAG_TIMEOUT         ((uint32_t)0x1000)

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


#define Encoder_TASK_PRIORITY				        ( tskIDLE_PRIORITY + 2UL )
 
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define SPIx_CS_LOW()       GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define SPIx_CS_HIGH()      GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

/************************** Function Prototypes ******************************/

void EncoderInitialise(void);
void EncoderRefershTask( void *pvParameters );

/* USER Callbacks: This is function for which prototype only is declared in
   MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* Encoder_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_Encoder.h file.
   Typically the user implementation of this callback should reset MEMS peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t SPIx_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_ENCODER_H */
