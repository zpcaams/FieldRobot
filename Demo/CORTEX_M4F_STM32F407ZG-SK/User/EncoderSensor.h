/**
  ******************************************************************************
  * @file    EncoderSensor.c
  * @author  ZhouPeng
  * @version V0.1
  * @date    2013-3-22
  * @brief   This file contains all the functions prototypes for the EncoderSensor.c
  *          firmware driver.
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODERSENSOR_H
#define __ENCODERSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   	 

/**
  * @}
  */
  
/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Exported_Constants
  * @{
  */

/* Uncomment the following line to use the default EncoderSensor_TIMEOUT_UserCallback() 
   function implemented in stm32f4_discovery_EncoderSensor.c file.
   EncoderSensor_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)). */   
 #define USE_DEFAULT_TIMEOUT_CALLBACK 

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EncoderSensor_FLAG_TIMEOUT         ((uint32_t)0x1000)

/**
  * @brief  EncoderSensor SPI Interface pins
  */
#define EncoderSensor_SPI                       SPI2
#define EncoderSensor_SPI_CLK                   RCC_APB1Periph_SPI2

#define EncoderSensor_SPI_SCK_PIN               GPIO_Pin_13                  /* PB.13 */
#define EncoderSensor_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define EncoderSensor_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define EncoderSensor_SPI_SCK_SOURCE            GPIO_PinSource13
#define EncoderSensor_SPI_SCK_AF                GPIO_AF_SPI2

#define EncoderSensor_SPI_MISO_PIN              GPIO_Pin_14                  /* PB.14 */
#define EncoderSensor_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define EncoderSensor_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define EncoderSensor_SPI_MISO_SOURCE           GPIO_PinSource14
#define EncoderSensor_SPI_MISO_AF               GPIO_AF_SPI1

#define EncoderSensor_SPI_MOSI_PIN              GPIO_Pin_15                  /* PB.15 */
#define EncoderSensor_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define EncoderSensor_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define EncoderSensor_SPI_MOSI_SOURCE           GPIO_PinSource15
#define EncoderSensor_SPI_MOSI_AF               GPIO_AF_SPI1

#define EncoderSensor_SPI_CS_PIN                GPIO_Pin_12                  /* PB.12 */
#define EncoderSensor_SPI_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define EncoderSensor_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Exported_Macros
  * @{
  */
#define EncoderSensor_CS_LOW()       GPIO_ResetBits(EncoderSensor_SPI_CS_GPIO_PORT, EncoderSensor_SPI_CS_PIN)
#define EncoderSensor_CS_HIGH()      GPIO_SetBits(EncoderSensor_SPI_CS_GPIO_PORT, EncoderSensor_SPI_CS_PIN)
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_EncoderSensor_Exported_Functions
  * @{
  */ 
void vEncoderSensorInitialise(void);
void vEncoderSensorRefershTask( void *pvParameters );

/* USER Callbacks: This is function for which prototype only is declared in
   MEMS accelerometre driver and that should be implemented into user applicaiton. */  
/* EncoderSensor_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting transmit data register empty flag(TXE)
   or waiting receive data register is not empty flag (RXNE)).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f4_discovery_EncoderSensor.h file.
   Typically the user implementation of this callback should reset MEMS peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t EncoderSensor_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __ENCODERSENSOR_H */
