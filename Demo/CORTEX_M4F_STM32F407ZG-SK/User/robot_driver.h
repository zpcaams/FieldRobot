/*****************************************************************************/
/**
*
* @file robot_debug.h
*
*
******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/
#include "main.h"

/************************** Constant Definitions *****************************/
/* #define USE_CAN1*/
#define USE_CAN1

#ifdef  USE_CAN1
  #define CANx                       CAN1
  #define CAN_CLK                    RCC_APB1Periph_CAN1
  #define CAN_RX_PIN                 GPIO_Pin_0
  #define CAN_TX_PIN                 GPIO_Pin_1
  #define CAN_GPIO_PORT              GPIOD
  #define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOD
  #define CAN_AF_PORT                GPIO_AF_CAN1
  #define CAN_RX_SOURCE              GPIO_PinSource0
  #define CAN_TX_SOURCE              GPIO_PinSource1       
#else /*USE_CAN2*/
  #define CANx                       CAN2
  #define CAN_CLK                    (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
  #define CAN_RX_PIN                 GPIO_Pin_5
  #define CAN_TX_PIN                 GPIO_Pin_13
  #define CAN_GPIO_PORT              GPIOB
  #define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOB
  #define CAN_AF_PORT                GPIO_AF_CAN2
  #define CAN_RX_SOURCE              GPIO_PinSource5
  #define CAN_TX_SOURCE              GPIO_PinSource13    
#endif  /* USE_CAN1 */

 /**************************** Type Definitions *******************************/

 /***************** Macros (Inline Functions) Definitions *********************/

 /************************** Function Prototypes ******************************/
void CANMsgRcvrfromIRQ(uint8_t FIFONumber);
void vCANConfigInitialise(void);
void vCANMainTask( void *pvParameters );
void Init_RxMes(CanRxMsg *RxMessage);
void CANMsgSendTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
