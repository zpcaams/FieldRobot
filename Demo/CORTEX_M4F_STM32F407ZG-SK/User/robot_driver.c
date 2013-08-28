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

extern xQueueHandle xCANRcvQueue, xCANTransQueue;

WheelMotor_TypeDef		WheelMotor[DirMax];
SteeringMotor_TypeDef	SteeringMotor[DirMax];
ElectricPutter_TypeDef	ElectricPutter[DirMax];

/*****************************************************************************/
/**
* Send Command to Driver
* 
* @param	DriverMsg_TypeDef *DriverMsg
*
* @return	None
*
* @note		None
*
******************************************************************************/
static void DriverSendCmd(DriverMsg_TypeDef *DriverMsg)
{
	u8 i;
	u32 Id;
    portBASE_TYPE xStatus;
	CanTxMsg CANTxMessage;
	CanRxMsg CANRxMessage;
	
	/* SetUp CAN Msg */
	Id = (u8)(DriverMsg->Base) + (u8)(DriverMsg->Dir);
    CANTxMessage.StdId = Id;
    CANTxMessage.ExtId = 0;
    CANTxMessage.IDE = CAN_ID_STD;
    CANTxMessage.RTR = CAN_RTR_DATA;
    CANTxMessage.DLC = DriverMsg->Len;
    CANTxMessage.Data[0] = DriverMsg->Len;
    CANTxMessage.Data[1] = Id;
    CANTxMessage.Data[2] = DriverMsg->Cmd;
    CANTxMessage.Data[3] = 0;
    
    for(i=4;i<(DriverMsg->Len);i++){
    	CANTxMessage.Data[i] = DriverMsg->TxData.U8[i-4];
    }
    
    /* Send CAN Msg */
SEND_CAN_MSG:
	xStatus = xQueueSendToBack(xCANTransQueue, &CANTxMessage, 0);
	
    /* Receive CAN Msg */
	xStatus = xQueueReceive( xCANRcvQueue, &CANRxMessage, 5/portTICK_RATE_MS);
	if (xStatus==pdPASS){
		if ((CANRxMessage.Data[1]==Id)&&
			(CANRxMessage.Data[2]==DriverMsg->Cmd)&&
			(CANRxMessage.Data[3]==MLDS_ACK)){
			for(i=0;i<(CANRxMessage.Data[0]-4);i++){
				DriverMsg->RxData.U8[i] = CANRxMessage.Data[i+4];
    			return;
			}
		}
	}
	goto SEND_CAN_MSG;
}

/*****************************************************************************/
/**
*
* Set/Get Motor Speed.
*
* @param  	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetMotorSpeed (DriverMsg_TypeDef *DriverMsg)
{
	WheelMotor[DriverMsg->Dir].V = DriverMsg->TxData.S32;
	
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_V;
SEND_SPEED_CMD:
	DriverSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_SPEED_CMD;
}

void GetMotorSpeed (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GV;
	DriverSendCmd(DriverMsg);
	if(DriverMsg->Base==WM_BASE){
		WheelMotor[DriverMsg->Dir].GV = DriverMsg->RxData.S32;		
	}else if(DriverMsg->Base==SM_BASE){
		/* Do Nothing */
	}else if(DriverMsg->Base==EP_BASE){
		/* Do Nothing */
	}
}

void GetMotorCurrent(DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GC;
	DriverSendCmd(DriverMsg);
	
	if(DriverMsg->Base==WM_BASE){
		WheelMotor[DriverMsg->Dir].GC = DriverMsg->RxData.S16;
	}else if(DriverMsg->Base==SM_BASE){
		SteeringMotor[DriverMsg->Dir].GC = DriverMsg->RxData.S16;
	}else if(DriverMsg->Base==EP_BASE){
		ElectricPutter[DriverMsg->Dir].GC = DriverMsg->RxData.S16;
	}
}

void GetMotorTemp (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GT;
	DriverSendCmd(DriverMsg);

	if(DriverMsg->Base==WM_BASE){
		WheelMotor[DriverMsg->Dir].GT = DriverMsg->RxData.S16;
	}else if(DriverMsg->Base==SM_BASE){
		SteeringMotor[DriverMsg->Dir].GT = DriverMsg->RxData.S16;
	}else if(DriverMsg->Base==EP_BASE){
		ElectricPutter[DriverMsg->Dir].GT = DriverMsg->RxData.S16;
	}
}

void GetMotorError (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GEI;
	DriverSendCmd(DriverMsg);

	if(DriverMsg->Base==WM_BASE){
		WheelMotor[DriverMsg->Dir].GEI = DriverMsg->RxData.U16;
	}else if(DriverMsg->Base==SM_BASE){
		SteeringMotor[DriverMsg->Dir].GEI = DriverMsg->RxData.U16;
	}else if(DriverMsg->Base==EP_BASE){
		ElectricPutter[DriverMsg->Dir].GEI = DriverMsg->RxData.U16;
	}
}

/*****************************************************************************/
/**
*
* Set/Get Motor Position, Move to New Position
*
* @param  	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetMotorPos (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_PO;
SEND_POS_CMD:
	DriverSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_POS_CMD;
}

void SetMotorMove (DriverMsg_TypeDef *DriverMsg)
{	
	SteeringMotor[DriverMsg->Dir].M = DriverMsg->TxData.S32;
	
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_M;
SEND_MOVE_CMD:
	DriverSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_MOVE_CMD;
}

void GetMotorPos (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GM;
	DriverSendCmd(DriverMsg);

	if(DriverMsg->Base==WM_BASE){
		/* Do Nothing */
	}else if(DriverMsg->Base==SM_BASE){
		SteeringMotor[DriverMsg->Dir].GM = DriverMsg->RxData.S32;
	}else if(DriverMsg->Base==EP_BASE){
		/* Do Nothing */
	}
}

void SetMotorPWM (DriverMsg_TypeDef *DriverMsg)
{	
	ElectricPutter[DriverMsg->Dir].AM = DriverMsg->TxData.S16;
	
	DriverMsg->Len = 6;
	DriverMsg->Cmd = MLDS_AM;
SEND_AM_CMD:
	DriverSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_AM_CMD;
}

void GetMotorType (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GDTY;
	DriverSendCmd(DriverMsg);
}

void DriverSelfTest(void)
{
	Dir_TypeDef Dir;
	u8 ErrorCounter = 0;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;	

TEST_RESET:
	if(ErrorCounter>10){
		goto TEST_FAILED;
	}
	
	pDriverMsg->Base = WM_BASE;
	
	for(Dir=DirMin;Dir<DirMax;Dir++){
		pDriverMsg->Dir = Dir;
		GetMotorType(pDriverMsg);
		if((pDriverMsg->RxData.U16)!=0x4E){
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
