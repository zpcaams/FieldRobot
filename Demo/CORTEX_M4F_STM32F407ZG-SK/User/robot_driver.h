/*****************************************************************************/
/**
*
* @file robot_debug.h
*
*
******************************************************************************/

#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

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

/* MLDS includes. */
#include "mlds_can.h"
 
#include "robot_status.h"

/************************** Constant Definitions *****************************/
/* #define USE_CAN1*/
#define USE_CAN1

#ifdef  USE_CAN1
  #define CANx                       CAN1
  #define CAN_CLK                    RCC_APB1Periph_CAN1
  #define CAN_RX_PIN                 GPIO_Pin_8
  #define CAN_TX_PIN                 GPIO_Pin_9
  #define CAN_GPIO_PORT              GPIOB
  #define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOB
  #define CAN_AF_PORT                GPIO_AF_CAN1
  #define CAN_RX_SOURCE              GPIO_PinSource8
  #define CAN_TX_SOURCE              GPIO_PinSource9       
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
 
/* CAN ID Base for Moto Drivers */
#define WheelMotorId     1
#define SteeringMotorId  5
#define ElectricPutterId 9
 
/* CAN Task Priority Setup */
#define CANMsgSend_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 4UL )
#define CANMain_TASK_PRIORITY				        ( tskIDLE_PRIORITY + 3UL )
   
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void CANMsgRcvrfromIRQ(uint8_t FIFONumber);
void CANInitialise(void);
void CANMainTask( void *pvParameters );
void Init_RxMes(CanRxMsg *RxMessage);
void CANMsgSendTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
