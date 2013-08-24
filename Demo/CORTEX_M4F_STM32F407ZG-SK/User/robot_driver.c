/*****************************************************************************/
/**
* @file robot_driver.c
*
*
* This file contains a moto driver control code via CAN.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_driver.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

xQueueHandle xCANRcvQueue, xCANTransQueue;

extern xSemaphoreHandle RobotStatusSemaphore;
extern WheelMotor_4TypeDef      WheelMotor;
extern SteeringMotor_4TypeDef   SteeringMotor;

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
* Send Command to Driver
* 
* @param	u32	Id			CAN Id of Target Driver
* 			u8	Len			Length of CAN Message
* 			u8	Cmd			Cmd to Sent, List in mlds_can.h
* 			u8	*pTxData	Pointer Points the Data to Sent
* 			s32	*pRxData	Pointer Points the Data wil Receive
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void CANSendCmd(DriverMsg_TypeDef *DriverMsg)
{
	u8 i;
    portBASE_TYPE xStatus;
	CanTxMsg CANTxMessage;
	CanRxMsg CANRxMessage;
	
	/* SetUp CAN Msg */
    CANTxMessage.StdId = (uint32_t)(DriverMsg->Id);
    CANTxMessage.ExtId = 0;
    CANTxMessage.IDE = CAN_ID_STD;
    CANTxMessage.RTR = CAN_RTR_DATA;
    CANTxMessage.DLC = DriverMsg->Len;
    CANTxMessage.Data[0] = DriverMsg->Len;
    CANTxMessage.Data[1] = DriverMsg->Id;
    CANTxMessage.Data[2] = DriverMsg->Cmd;
    CANTxMessage.Data[3] = 0;
    
    for(i=4;i<(DriverMsg->Len);i++){
    	CANTxMessage.Data[i] = *((DriverMsg->TxData)+i-4);
    }
    
    /* Send CAN Msg */
SEND_CAN_MSG:
	xStatus = xQueueSendToBack(xCANTransQueue, &CANTxMessage, 0);
	
    /* Receive CAN Msg */
	xStatus = xQueueReceive( xCANRcvQueue, &CANRxMessage, 5/portTICK_RATE_MS);
	if (xStatus==pdPASS){
		if ((CANRxMessage.Data[1]==DriverMsg->Id)&&
			(CANRxMessage.Data[2]==DriverMsg->Cmd)&&
			(CANRxMessage.Data[3]==MLDS_ACK)){
			for(i=0;i<4;i++){
				DriverMsg->RxData[i] = CANRxMessage.Data[i+4];
    			return;
			}
		}
	}
	goto SEND_CAN_MSG;
}

/*****************************************************************************/
/**
*
* Setup Moto Speed through CAN bus.
*
* @param  	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetMotoSpeed (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_V;
	CANSendCmd(DriverMsg);
}

void GetMotoSpeed (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GV;
	CANSendCmd(DriverMsg);
}

void GetMotoCurrent(DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GC;
	CANSendCmd(DriverMsg);
}

void GetMotoTemp (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GT;
	CANSendCmd(DriverMsg);
}

void GetMotoError (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GEI;
	CANSendCmd(DriverMsg);
}

/*****************************************************************************/
/**
*
* Setup Steering Moto Position through CAN bus.
* Position info from Remote Control Channel 1.
*
* @param  	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetMotoPos (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_M;
	CANSendCmd(DriverMsg);
}

void GetMotoPos (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GM;
	CANSendCmd(DriverMsg);
}

void GetMotoType (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GDTY;
	CANSendCmd(DriverMsg);
}

void CANSelfTest(void)
{
	u8 i;
	u8 ErrorCounter = 0;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;	

TEST_RESET:
	if(ErrorCounter>10){
		goto TEST_FAILED;
	}
	
	for(i=WheelMotorId;i<(WheelMotorId+4);i++){
		pDriverMsg->Id = i;
		GetMotoType(pDriverMsg);
		if((pDriverMsg->RxData[0])!=0x4E){
			ErrorCounter++;
			goto TEST_RESET;
		}
	}
	DebugPrintf("CAN Selftest pass!\n");
	return;
	
TEST_FAILED:
	DebugPrintf("CAN Selftest failed!\n");
	while(1);
}

/*****************************************************************************/
/**
*
* Steering Motor Position Initialize.
*
* @param  	None
*
* @return	None
*
* @note		1:Get the Position from Encoder.
* 			2:Set the Position using PO command.
*
******************************************************************************/
void SteeringMotorPosInitTask(void *pvParameters)
{
	u8 i;
    portTickType xLastWakeTime;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
	
    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){

		for(i=SteeringMotorId;i<(SteeringMotorId+4);i++){
			pDriverMsg->Id = i;
			*(s32 *)(&(pDriverMsg->RxData[0])) = GetSteeringMotorPosition(i-SteeringMotorId);
			SetMotoPos(pDriverMsg);
			if((pDriverMsg->RxData[0])!=0){
				/* Error */
			}
		}
		
		DebugPrintf("Steering Motor Initialize Done!\n");
		xSemaphoreGive(RobotStatusSemaphore);
		vTaskDelete(NULL);
		
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
	}
}

/*****************************************************************************/
/**
*
* Steering Motor Position Test Task.
*
* @param  	None
*
* @return	None
*
* @note		Print Position Get from Encoder and Driver.
*
******************************************************************************/
void SteeringMotorPosTestTask(void *pvParameters)
{
	u8 i;
    portTickType xLastWakeTime;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
    u16 AbsEncoderInt;
	s32 SteeringMotorRightFrontEncoderValue;
	s32 SteeringMotorRightFrontDriverValue;

    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){
		
		for(i=SteeringMotorId;i<(SteeringMotorId+4);i++){
			
			pDriverMsg->Id = i;
			GetMotoPos(pDriverMsg);
			SteeringMotorRightFrontDriverValue = *(s32 *)(&(pDriverMsg->RxData[0]));
			SteeringMotorRightFrontEncoderValue=GetSteeringMotorPosition(i-SteeringMotorId);
			AbsEncoderInt = GetAbsEncoderInt(i-SteeringMotorId);

			SteeringMotorRightFrontEncoderValue/=4;
			SteeringMotorRightFrontDriverValue/=4;
	    
			DebugPrintf("SMRF:%i %i %i\n", SteeringMotorRightFrontEncoderValue, 
					SteeringMotorRightFrontDriverValue, AbsEncoderInt);
		}
		vTaskDelayUntil( &xLastWakeTime, 300 / portTICK_RATE_MS );
	}
}

/*****************************************************************************/
/**
*
* Main CAN Task, run every 10 ms.
*		small tasks
*		1. give the wheel motor current
*		2. get the wheel motor speed
*		3. give the Steering motor position
*		4. get the Steering motor position
*		5. give the Motorized Faders voltage
*		6. get the Motorized Faders position
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void CANMainTask( void *pvParameters )
{
    portTickType xLastWakeTime;
    uint8_t TaskCounter = 0;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        if (TaskCounter<11) {
            TaskCounter++;
        } else {
            TaskCounter = 0;
        }
		DebugPrintf("%i ", TaskCounter); 
      
//        switch (TaskCounter){
//            case 0: {
//            	SetWheelMotoSpeed();
//                break;
//            }
//            case 1: {
//            	GetWheelMotoSpeed();
//                break;
//            }
//            case 2: {
//            	GetWheelMotoCurrent();
//                break;
//            }
//            case 3: {
//            	GetWheelMotoTemp();
//                break;
//            }
//            case 4: {
//            	GetWheelMotoError();
//                break;
//            }
//            
//            case 5:{
//            	SetSteeringMotorPos();
//                break;
//            }
//            case 6: {
//            	GetSteeringMotoSpeed();
//                break;
//            }
//            case 7: {
//            	GetSteeringMotoCurrent();
//                break;
//            }
//            case 8: {
//            	GetSteeringMotoTemp();
//                break;
//            }
//            case 9: {
//            	GetSteeringMotoError();
//                break;
//            }
//            default:{
//                break;
//            }
//        }
        
    /* Run this task every 10 ms */
    vTaskDelayUntil( &xLastWakeTime, 10 / portTICK_RATE_MS );
    
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

/*****************************************************************************/
/**
*
* Initializes the Rx Message.
*
* @param	RxMessage: pointer to the message to initialize
*
* @return	None
*
* @note		None
*
******************************************************************************/
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
