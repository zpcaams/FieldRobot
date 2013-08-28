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

extern	xSemaphoreHandle RobotStatusSemaphore;

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
	Dir_TypeDef Dir;
    portTickType xLastWakeTime;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
	
    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){

		pDriverMsg->Base = SM_BASE;
		for(Dir=DirMin;Dir<DirMax;Dir++){
			pDriverMsg->Dir = Dir;
			pDriverMsg->TxData.S32 = GetSterringMotorPosition(Dir);
			SetMotorPos(pDriverMsg);
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