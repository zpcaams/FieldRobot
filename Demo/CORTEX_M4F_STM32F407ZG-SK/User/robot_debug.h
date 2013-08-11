/*****************************************************************************/
/**
*
* @file robot_debug.h
*
*
******************************************************************************/

#ifndef ROBOT_DEBUG_H
#define ROBOT_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"

#include <stdarg.h>

/************************** Constant Definitions *****************************/
#define USARTx                       USART3
#define USART_CLK                    RCC_APB1Periph_USART3
#define USART_RX_PIN                 GPIO_Pin_9
#define USART_TX_PIN                 GPIO_Pin_8
#define USART_GPIO_PORT              GPIOD
#define USART_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define USART_AF_PORT                GPIO_AF_USART3
#define USART_RX_SOURCE              GPIO_PinSource9
#define USART_TX_SOURCE              GPIO_PinSource8    

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void DebugInitialise( void );
void DebugPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DEBUG_H */
