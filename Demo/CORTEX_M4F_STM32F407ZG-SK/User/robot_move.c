/*****************************************************************************/
/**
* @file robot_move.c
*
*
* This file contains robot move control code.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_move.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern u8		RobotStatus;
extern WheelMotor_4TypeDef      WheelMotor;
extern SteeringMotor_4TypeDef   SteeringMotor;
/*****************************************************************************/
/**
*
* Robot Move Task
*
* @param  	None
*
* @return	None
*
* @note		None
*
******************************************************************************/
void EnterRobotMoveStatus (void)
{
	RobotStatus = ROBOT_MOVE;
	xTaskCreate( RobotMoveTask, ( signed char * ) "RbtMv",
			configMINIMAL_STACK_SIZE, NULL, RobotMove_TASK_PRIORITY, NULL );
}
void EnterRobotMoveStopStatus (void)
{
	RobotStatus = ROBOT_MOVE_STOP;
}

void RobotMoveTask (void *pvParameters)
{
	s32 Speed;
	s32 Position;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {    
        /* calculate the speed here */
		Speed = (GetRemoteControl(3-1)-1440)/8;
		if((Speed<5)&&(Speed>-5)){
			Speed = 0;
		}
		DebugPrintf("Speed %i\n", Speed);
		
		pDriverMsg->Id = WheelMotorId + PosRightFront;
		*(s32 *)(&(pDriverMsg->RxData[0])) = Speed;
		SetMotoSpeed (pDriverMsg);
		GetMotoSpeed (pDriverMsg);
		GetMotoCurrent (pDriverMsg);
		GetMotoTemp (pDriverMsg);
		GetMotoError (pDriverMsg);
		
	    /* Calculate the Position here */
	    Position = -(GetRemoteControl(1-1)-1460);
		if((Position<5)&&(Position>-5)){
			Position = 0;
		}	    
	    DebugPrintf("Pos %i\n", Position);
	    
		pDriverMsg->Id = SteeringMotorId + PosRightFront;
		*(s32 *)(&(pDriverMsg->RxData[0])) = Position;
		SetMotoPos (pDriverMsg);
		GetMotoSpeed (pDriverMsg);
		GetMotoCurrent (pDriverMsg);
		GetMotoTemp (pDriverMsg);
		GetMotoError (pDriverMsg);
		
//		DebugPrintf("RobotMoveTask is running\n");
		
        if(RobotStatus==ROBOT_MOVE_STOP){
			xTaskCreate( RobotMoveStopTask, ( signed char * ) "RbtMvStp",
					configMINIMAL_STACK_SIZE, NULL, RobotMoveStop_TASK_PRIORITY, NULL );
			vTaskDelete(NULL);
        }
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }   
}

void RobotMoveStopTask (void *pvParameters)
{
	s32 Speed;
	s32 Position;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
		Speed = 0;
		pDriverMsg->Id = WheelMotorId + PosRightFront;
		*(s32 *)(&(pDriverMsg->RxData[0])) = Speed;
		SetMotoSpeed (pDriverMsg);
		GetMotoSpeed (pDriverMsg);
		GetMotoCurrent (pDriverMsg);
		GetMotoTemp (pDriverMsg);
		GetMotoError (pDriverMsg);
	    
		Position = 0;
		pDriverMsg->Id = SteeringMotorId + PosRightFront;
		*(s32 *)(&(pDriverMsg->RxData[0])) = Position;
		SetMotoPos (pDriverMsg);
		GetMotoSpeed (pDriverMsg);
		GetMotoCurrent (pDriverMsg);
		GetMotoTemp (pDriverMsg);
		GetMotoError (pDriverMsg);
		
//		DebugPrintf("RobotMoveStopTask is running\n");
		
		if((WheelMotor.RightFront.GV==0)&&(SteeringMotor.RightFront.GP==0)){
	    	ResetRoborBusy();
			vTaskDelete(NULL);
		}
        
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }   
}