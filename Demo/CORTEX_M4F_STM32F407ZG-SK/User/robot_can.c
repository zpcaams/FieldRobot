/*****************************************************************************/
/**
* @file robot_can.c
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_can.h"

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

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

xQueueHandle xCANRcvQueue, xCANTransQueue;

/*****************************************************************************/
/**
*
* CAN Send Message Task
*
* @param  	None
*
* @return	None
*
* @note		Task Unblock when Queue is NOT Empty.
* 			Send the Message to CAN bus.
*
******************************************************************************/
static void CANMsgSendTask (void *pvParameters)
{
	CanTxMsg CANTxMsgFromQueue;

    for(;;){
        if (xQueueReceive(xCANTransQueue, &CANTxMsgFromQueue, portMAX_DELAY)==pdPASS){
			/* 
			 * Under the Robot Task control, MailBox is not able to Full.
			 * So We Don't Check Return Here.
			 */
			CAN_Transmit(CANx, &CANTxMsgFromQueue);
        }
    }
}

/*****************************************************************************/
/**
*
* Configures the NVIC for CAN.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void NVIC_Config_CAN(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************/
/**
*
* Configures the CAN Peripher, Interrupt, Queue and Task.
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void CANInitialise(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  CAN_InitTypeDef         CAN_InitStructure;
  CAN_FilterInitTypeDef   CAN_FilterInitStructure; 
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = ENABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN Baudrate = PCLK1/((1+CAN_BS1+CAN_BS2)*CAN_Prescaler) 
                  = 500KBps (CAN clocked at 42 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
  CAN_InitStructure.CAN_Prescaler = 7;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
  NVIC_Config_CAN();
  
  /* Creat the queue for CAN */
  xCANRcvQueue = xQueueCreate( 8, sizeof(CanRxMsg) );
  xCANTransQueue = xQueueCreate( 4, sizeof(CanTxMsg) );
  	
  /* Creat the CAN Semaphore and Send/Receive Task */
  xTaskCreate( CANMsgSendTask, ( signed char * ) "Can1Send", 
		  configMINIMAL_STACK_SIZE, NULL, CANMsgSend_TASK_PRIORITY, NULL );
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
