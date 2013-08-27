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
    	CANTxMessage.Data[i] = DriverMsg->TxData.U8[i-4];
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
	WheelMotor[(DriverMsg->Id)-WheelMotorId].V = DriverMsg.TxData.S32;
	
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_V;
SEND_SPEED_CMD:
	CANSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_SPEED_CMD;
}

void GetMotorSpeed (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GV;
	CANSendCmd(DriverMsg);
	if((DriverMsg->Id)<(WheelMotorId)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(WheelMotorId+DirMax)){
		WheelMotor[(DriverMsg->Id)-WheelMotorId].GV = DriverMsg->RxData.S32;
	}else if((DriverMsg->Id)<(SteeringMotorId+DirMax)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(ElectricPutterId+DirMax)){
		/* Do Nothing */
	}else{
		/* Do Nothing */
	}
}

void GetMotorCurrent(DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GC;
	CANSendCmd(DriverMsg);
	if((DriverMsg->Id)<(WheelMotorId)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(WheelMotorId+DirMax)){
		WheelMotor[(DriverMsg->Id)-WheelMotorId].GC = DriverMsg->RxData.S16;
	}else if((DriverMsg->Id)<(SteeringMotorId+DirMax)){
		SteeringMotor[(DriverMsg->Id)-SteeringMotorId].GC = DriverMsg->RxData.S16;
	}else if((DriverMsg->Id)<(ElectricPutterId+DirMax)){
		ElectricPutter[(DriverMsg->Id)-ElectricPutterId].GC = DriverMsg->RxData.S16;
	}else{
		/* Do Nothing */
	}
}

void GetMotorTemp (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GT;
	CANSendCmd(DriverMsg);
	if((DriverMsg->Id)<(WheelMotorId)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(WheelMotorId+DirMax)){
		WheelMotor[(DriverMsg->Id)-WheelMotorId].GT = DriverMsg->RxData.S16;
	}else if((DriverMsg->Id)<(SteeringMotorId+DirMax)){
		SteeringMotor[(DriverMsg->Id)-SteeringMotorId].GT = DriverMsg->RxData.S16;
	}else if((DriverMsg->Id)<(ElectricPutterId+DirMax)){
		ElectricPutter[(DriverMsg->Id)-ElectricPutterId].GT = DriverMsg->RxData.S16;
	}else{
		/* Do Nothing */
	}
}

void GetMotorError (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GEI;
	CANSendCmd(DriverMsg);
	if((DriverMsg->Id)<(WheelMotorId)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(WheelMotorId+DirMax)){
		WheelMotor[(DriverMsg->Id)-WheelMotorId].GEI = DriverMsg->RxData.U16;
	}else if((DriverMsg->Id)<(SteeringMotorId+DirMax)){
		SteeringMotor[(DriverMsg->Id)-SteeringMotorId].GEI = DriverMsg->RxData.U16;
	}else if((DriverMsg->Id)<(ElectricPutterId+DirMax)){
		ElectricPutter[(DriverMsg->Id)-ElectricPutterId].GEI = DriverMsg->RxData.U16;
	}else{
		/* Do Nothing */
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
	CANSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_POS_CMD;
}

void SetMotorMove (DriverMsg_TypeDef *DriverMsg)
{	
	WheelMotor[(DriverMsg->Id)-SteeringMotorId].M = DriverMsg.TxData.S32;
	
	DriverMsg->Len = 8;
	DriverMsg->Cmd = MLDS_M;
SEND_MOVE_CMD:
	CANSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_MOVE_CMD;
}

void GetMotorPos (DriverMsg_TypeDef *DriverMsg)
{
	DriverMsg->Len = 4;
	DriverMsg->Cmd = MLDS_GM;
	CANSendCmd(DriverMsg);
	if((DriverMsg->Id)<(WheelMotorId)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(WheelMotorId+DirMax)){
		/* Do Nothing */
	}else if((DriverMsg->Id)<(SteeringMotorId+DirMax)){
		SteeringMotor[(DriverMsg->Id)-SteeringMotorId].GM = DriverMsg->RxData.S32;
	}else if((DriverMsg->Id)<(ElectricPutterId+DirMax)){
		/* Do Nothing */
	}else{
		/* Do Nothing */
	}
}

void SetMotorPWM (DriverMsg_TypeDef *DriverMsg)
{	
	ElectricPutter[(DriverMsg->Id)-SteeringMotorId].AM = DriverMsg.TxData.S16;
	
	DriverMsg->Len = 6;
	DriverMsg->Cmd = MLDS_AM;
SEND_AM_CMD:
	CANSendCmd(DriverMsg);
	if((DriverMsg->RxData.S16)==0){
		return;
	}
	goto SEND_AM_CMD;
}

void GetMotorType (DriverMsg_TypeDef *DriverMsg)
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
		GetMotorType(pDriverMsg);
		if((pDriverMsg->RxData.U8[0])!=0x4E){
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
