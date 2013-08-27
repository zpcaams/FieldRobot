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
static void DriverSendCmd(DriverMsg_TypeDef *DriverMsg)
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

void DriverSelfTest(void)
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
