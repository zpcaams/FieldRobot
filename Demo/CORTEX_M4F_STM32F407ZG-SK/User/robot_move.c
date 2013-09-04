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
extern WheelMotor_TypeDef		WheelMotor[DirMax];
extern SteeringMotor_TypeDef	SteeringMotor[DirMax];

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
		if((Speed<8)&&(Speed>-8)){
			Speed = 0;
		}
		DebugPrintf("Speed %i\n", Speed);

		pDriverMsg->Base = WM_BASE;
		pDriverMsg->Dir = RightFront;
		pDriverMsg->TxData.S32 = Speed;
		SetMotorSpeed (pDriverMsg);
		GetMotorSpeed (pDriverMsg);
		GetMotorCurrent (pDriverMsg);
		GetMotorTemp (pDriverMsg);
		GetMotorError (pDriverMsg);
		
	    /* Calculate the Position here */
	    Position = -(GetRemoteControl(1-1)-1460);
		if((Position<5)&&(Position>-5)){
			Position = 0;
		}
		pDriverMsg->Base = SM_BASE;
		pDriverMsg->Dir = RightFront;
		pDriverMsg->TxData.S32 = Position;
		SetMotorMove (pDriverMsg);
		GetMotorCurrent (pDriverMsg);
		GetMotorTemp (pDriverMsg);
		GetMotorError (pDriverMsg);
	    DebugPrintf("E%i S%i G%i C%i\n", 
	    		GetSterringMotorPosition(RightFront), 
	    		Position,
	    		SteeringMotor[RightFront].GM,
	    		SteeringMotor[RightFront].GC);
	    
		
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
		pDriverMsg->Base = WM_BASE;
		pDriverMsg->Dir = RightFront;
		pDriverMsg->TxData.S32 = Speed;
		SetMotorSpeed (pDriverMsg);
		GetMotorSpeed (pDriverMsg);
		GetMotorCurrent (pDriverMsg);
		GetMotorTemp (pDriverMsg);
		GetMotorError (pDriverMsg);
		
		Position = 0;
		pDriverMsg->Base = SM_BASE;
		pDriverMsg->Dir = RightFront;
		pDriverMsg->TxData.S32 = Position;
		SetMotorPos (pDriverMsg);
		GetMotorSpeed (pDriverMsg);
		GetMotorCurrent (pDriverMsg);
		GetMotorTemp (pDriverMsg);
		GetMotorError (pDriverMsg);
		
//		DebugPrintf("RobotMoveStopTask is running\n");
		
		if((WheelMotor[RightFront].GV==0)&&(SteeringMotor[RightFront].GM==0)){
	    	ResetRoborBusy();
			vTaskDelete(NULL);
		}
        
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }   
}