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
#include "semphr.h"

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
#define WheelMotorId     4
#define SteeringMotorId  8
#define ElectricPutterId 12
 
/* CAN Task Priority Setup */
#define CANMsgSend_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 4UL )
#define CANMsgRcvr_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 4UL )
#define PositionInit_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 3UL )
#define CANMain_TASK_PRIORITY				        ( tskIDLE_PRIORITY + 3UL )
   
/**************************** Type Definitions *******************************/
 
 typedef struct
 {
 	u8 	Id;			/* Driver Id */
 	u8 	Len;		/* CAN Msg Length */
 	u8 	Cmd;		/* Msg Command */
 	u8 	TxData[4];	/* TxData */
 	u8 	RxData[4];	/* RxData */
 } DriverMsg_TypeDef;
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void CANInitialise(void);
xSemaphoreHandle GetCAN1RX0Semaphore(void);
void Init_RxMes(CanRxMsg *RxMessage);
void CANSelfTest(void);
void CANMainTask( void *pvParameters );
void SteeringMotorPosInitTask(void *pvParameters);
void SteeringMotorPosTestTask(void *pvParameters);

void SetMotoSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotoSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotoCurrent (DriverMsg_TypeDef *DriverMsg);
void GetMotoTemp (DriverMsg_TypeDef *DriverMsg);
void GetMotoError (DriverMsg_TypeDef *DriverMsg);
void SetMotoPos (DriverMsg_TypeDef *DriverMsg);
void GetMotoPos (DriverMsg_TypeDef *DriverMsg);
void GetMotoType (DriverMsg_TypeDef *DriverMsg);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
