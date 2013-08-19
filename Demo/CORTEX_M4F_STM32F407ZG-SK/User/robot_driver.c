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
* @param	Id		CAN Id of Target Driver
* 			Len		Length of CAN Message
* 			Cmd		Cmd to Sent, List in mlds_can.h
* 			pData	Pointer Points Data to Sent
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void CANSendCmd(u32 Id, u8 Len, u8 Cmd, u8 *pTxData, s32 *pRxData)
{
	u8 i;
    portBASE_TYPE xStatus;
	CanTxMsg CANTxMessage;
	CanRxMsg CANRxMessage;
	
	/* SetUp CAN Msg */
    CANTxMessage.StdId = Id;
    CANTxMessage.ExtId = 0;
    CANTxMessage.IDE = CAN_ID_STD;
    CANTxMessage.RTR = CAN_RTR_DATA;
    CANTxMessage.DLC = Len;
    CANTxMessage.Data[0] = Len;
    CANTxMessage.Data[1] = Id;
    CANTxMessage.Data[2] = Cmd;
    CANTxMessage.Data[3] = 0;
    
    for(i=4;i<Len;i++){
    	CANTxMessage.Data[i] = *(pTxData+i-4);
    }
    
    /* Send CAN Msg */
SEND_CAN_MSG:
	xStatus = xQueueSendToBack(xCANTransQueue, &CANTxMessage, 0);
	
    /* Receive CAN Msg */
	xStatus = xQueueReceive( xCANRcvQueue, &CANRxMessage, 5/portTICK_RATE_MS);
	if (xStatus==pdPASS){
		if ((CANRxMessage.Data[1]==Id)&&
			(CANRxMessage.Data[2]==Cmd)&&
			(CANRxMessage.Data[3]==MLDS_ACK)){
        *pRxData = *(s32 *)(&CANRxMessage.Data[4]);
      }
    			return;
	}
	goto SEND_CAN_MSG;
}

/*****************************************************************************/
/**
*
* @param	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void CANSelfTest(void)
{
	u32 Id;
	u8 Len;
	u8 Data;
	u8 i;

	Id = SteeringMotorId+PosRightFront;
	Len = 4;
	for(i=0;i<2;i++){
		CANSendCmd(Id, Len, MLDS_GDTY, NULL, (s32 *)(&Data));
		if(Data==0x4E){
			DebugPrintf("CAN Selftest pass!\n");
			return;
		}
		DebugPrintf("CAN Selftest failed, test again.\n");
	}
	
    DebugPrintf("CAN Selftest failed!\n");
	while(1);
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
void SetWheelMotoSpeed ( void ) 
{
	u32 Id;
	u8 Len;
	s32 Speed;
	s16 Ack;
    uint16_t RemoteChannel_2;
    
    /*
        calculate the speed here
    */
    RemoteChannel_2=GetRemoteControl(2-1);
    Speed = (RemoteChannel_2-1440)/8;
    if((Speed<3)&&(Speed>-3)){
        Speed=0;
    }
    DebugPrintf("Speed %i\n", Speed);    
		
    /* Send the message to driver */
	Id = WheelMotorId+PosRightFront;
	Len = 8;
	
	CANSendCmd(Id, Len, MLDS_V, (u8 *)(&Speed), (s32 *)(&Ack));
	if(Ack!=0){
		DebugPrintf("Ack Error\n"); 
	}
}

void GetWheelMotoSpeed (void) 
{
	u32 Id;
	u8 Len;
	s32 Speed;
    
	Id = WheelMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GV, NULL, &Speed);
	WheelMotor.RightFront.GV = Speed;
	DebugPrintf("Spd %i\n", Speed); 
}

void GetWheelMotoCurrent (void) 
{
	u32 Id;
	u8 Len;
	s32 Current;
    
	Id = WheelMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GC, NULL, &Current);
	WheelMotor.RightFront.GC = Current;
	DebugPrintf("Crt %i\n", Current); 
}

void GetWheelMotoTemp (void) 
{
	u32 Id;
	u8 Len;
	s16 Temp;
    
	Id = WheelMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GT, NULL, (s32 *)(&Temp));
	WheelMotor.RightFront.GT = Temp;
	DebugPrintf("Tmp %i\n", Temp); 
}

void GetWheelMotoError (void) 
{
	u32 Id;
	u8 Len;
	u16 Error;
    
	Id = WheelMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GEI, NULL, (s32 *)(&Error));
	WheelMotor.RightFront.GEI = Error;
	DebugPrintf("Err %x\n", Error); 
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
void SetSteeringMotorPos ( void ) 
{
	u32 Id;
	u8 Len;
	s32 Position;
	s16 Ack;
    uint16_t RemoteChannel_1;
    
    /* Calculate the Position here */
    RemoteChannel_1=GetRemoteControl(1-1);
    Position = -(RemoteChannel_1-1462);
    
    DebugPrintf("Pos %i\n", Position);    
		
    Id = SteeringMotorId+PosRightFront;
    Len = 8;
	
	CANSendCmd(Id, Len, MLDS_M, (u8 *)(&Position), (s32 *)(&Ack));
	if(Ack!=0){
		DebugPrintf("Ack Error\n"); 
	}
}

void GetSteeringMotoSpeed (void) 
{
	u32 Id;
	u8 Len;
	s32 Speed;
    
	Id = SteeringMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GV, NULL, &Speed);
	SteeringMotor.RightFront.GV = Speed;
	DebugPrintf("Spd %i\n", Speed); 
}

void GetSteeringMotoCurrent (void) 
{
	u32 Id;
	u8 Len;
	s32 Current;
    
	Id = SteeringMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GC, NULL, &Current);
	SteeringMotor.RightFront.GC = Current;
	DebugPrintf("Crt %i\n", Current); 
}

void GetSteeringMotoTemp (void) 
{
	u32 Id;
	u8 Len;
	s16 Temp;
    
	Id = SteeringMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GT, NULL, (s32 *)(&Temp));
	SteeringMotor.RightFront.GT = Temp;
	DebugPrintf("Tmp %i\n", Temp); 
}

void GetSteeringMotoError (void) 
{
	u32 Id;
	u8 Len;
	u16 Error;
    
	Id = SteeringMotorId+PosRightFront;
	Len = 4;
	
	CANSendCmd(Id, Len, MLDS_GEI, NULL, (s32 *)(&Error));
	SteeringMotor.RightFront.GEI = Error;
	DebugPrintf("Err %x\n", Error); 
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
void SteeringMotorPosInitializeTask(void *pvParameters)
{
    portTickType xLastWakeTime;
	u32 Id;
	u8 Len;
	s32 Position;
	s16 Ack;

    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){
		
		Position=GetSteeringMotorPosition(PosRightFront);

	    Id = SteeringMotorId+PosRightFront;
		Len=8;
		
	SEND_CMD:
		CANSendCmd(Id, Len, MLDS_PO, (u8 *)(&Position), (s32 *)(&Ack));
		if(Ack!=0){
			goto SEND_CMD;
		}
		
		DebugPrintf("Right Front Steering Motor Initialize Done!\n");
		xSemaphoreGive(GetRobotMainSemaphore());
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
    portTickType xLastWakeTime;
	u32 Id;
	u8 Len;
  s16 Ack;
	
    u16 AbsEncoderInt;
	s32 SteeringMotorRightFrontEncoderValue;
	s32 SteeringMotorRightFrontDriverValue;

    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){
		
	    Id = SteeringMotorId+PosRightFront;
		Len=4;
		
	SEND_CMD:
		CANSendCmd(Id, Len, MLDS_GM, (u8 *)(&SteeringMotorRightFrontDriverValue), (s32 *)(&Ack));
		if(Ack!=0){
			goto SEND_CMD;
		}

		SteeringMotorRightFrontEncoderValue=GetSteeringMotorPosition(PosRightFront);
		AbsEncoderInt = GetAbsEncoderInt(PosRightFront);
		
		SteeringMotorRightFrontEncoderValue/=4;
		SteeringMotorRightFrontDriverValue/=4;
    
		DebugPrintf("SMRF:%i %i %i\n", SteeringMotorRightFrontEncoderValue, 
				SteeringMotorRightFrontDriverValue, AbsEncoderInt);
		
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
      
        switch (TaskCounter){
            case 0: {
            	SetWheelMotoSpeed();
                break;
            }
            case 1: {
            	GetWheelMotoSpeed();
                break;
            }
            case 2: {
            	GetWheelMotoCurrent();
                break;
            }
            case 3: {
            	GetWheelMotoTemp();
                break;
            }
            case 4: {
            	GetWheelMotoError();
                break;
            }
            
            case 5:{
            	SetSteeringMotorPos();
                break;
            }
            case 6: {
            	GetSteeringMotoSpeed();
                break;
            }
            case 7: {
            	GetSteeringMotoCurrent();
                break;
            }
            case 8: {
            	GetSteeringMotoTemp();
                break;
            }
            case 9: {
            	GetSteeringMotoError();
                break;
            }
            default:{
                break;
            }
        }
        
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
