/*****************************************************************************/
/**
* @file robot_init.c
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
#include "robot_init.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/



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
	s32 Position;
	Dir_TypeDef Direction;
	
    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){

		for(i=SteeringMotorId;i<(SteeringMotorId+DirMax);i++){
			Direction = i-SteeringMotorId;
			pDriverMsg->Id = i;
			pDriverMsg->TxData.S32 = GetSteeringMotorPosition(Direction);
			SetMotoPos(pDriverMsg);
			if((pDriverMsg->RxData.S16)!=0){
				/* Error */
				DebugPrintf("Steering Motor Initialize Failed!\n");
				while(1);
			}
		}
		
		DebugPrintf("Steering Motor Initialize Done!\n");
		xSemaphoreGive(RobotStatusSemaphore);
		vTaskDelete(NULL);
		
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
	}
}