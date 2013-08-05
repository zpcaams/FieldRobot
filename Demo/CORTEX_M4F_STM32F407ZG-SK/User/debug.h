/*
 * debug.h
 *
 *  Created on: Feb 9, 2011
 *      Author: James
 */

#ifndef DEBUG_H_
#define DEBUG_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
  #define USARTx                       USART3
  #define USART_CLK                    RCC_APB1Periph_USART3
  #define USART_RX_PIN                 GPIO_Pin_9
  #define USART_TX_PIN                 GPIO_Pin_8
  #define USART_GPIO_PORT              GPIOD
  #define USART_GPIO_CLK               RCC_AHB1Periph_GPIOD
  #define USART_AF_PORT                GPIO_AF_USART3
  #define USART_RX_SOURCE              GPIO_PinSource9
  #define USART_TX_SOURCE              GPIO_PinSource8    

/* Exported functions ------------------------------------------------------- */
void vDebugInitialise( void );
void vDebugInitQueue( void );
void vDebugString( char *s );
void vDebugPrintf(const char *fmt, ...);
void vDebugPrintResetType( void );


#endif /* DEBUG_H_ */
