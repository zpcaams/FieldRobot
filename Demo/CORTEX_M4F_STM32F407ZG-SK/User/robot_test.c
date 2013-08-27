/*****************************************************************************/
/**
* @file robot_test.c
*
*
* This file contains robot test code.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_test.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern u8						RobotStatus;
extern u16						RemoteControl[8];

u8 CouplingsSellect (void)
{
	if((RemoteControl[3-1]>1189)&&(RemoteControl[3-1]<1742)){
		return (RemoteControl[6-1]-RemoteControl[3-1]+75);
	}else{
		return 0;
	}
}

void EnterRobotTestStatus (void)
{
	RobotStatus = ROBOT_TEST;
	xTaskCreate( RobotTestTask, ( signed char * ) "RbtTst",
			configMINIMAL_STACK_SIZE, NULL, RobotTest_TASK_PRIORITY, NULL );
}
void EnterRobotTestStopStatus (void)
{
	RobotStatus = ROBOT_TEST_STOP;
}

void RobotTestTask (void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    u8 temp;

    for( ; ; )
    {
		temp = CouplingsSellect();
		
    	if(temp<25){
    		CouplingsOff();
		}else if(temp<50){
    		CouplingsOn(PosLeftBack);
    	}else if(temp<75){
    		CouplingsOn(PosRightBack);
    	}else if(temp<100){
    		CouplingsOn(PosRightFront);
    	}else if(temp<125){
    		CouplingsOn(PosLeftFront);
    	}else{
    		CouplingsOff();
    	}
		
        if(RobotStatus==ROBOT_TEST_STOP){
			xTaskCreate( RobotTestStopTask, ( signed char * ) "RbtTstStp",
					configMINIMAL_STACK_SIZE, NULL, RobotTestStop_TASK_PRIORITY, NULL );
			vTaskDelete(NULL);
        }
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }   
}

void RobotTestStopTask (void *pvParameters)
{
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
		CouplingsOff();
		ResetRoborBusy();
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
	s32 SteeringMotorRightFrontEncoderValue;
	s32 SteeringMotorRightFrontDriverValue;

    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){
		
		for(i=SteeringMotorId;i<(SteeringMotorId+4);i++){
			
			pDriverMsg->Id = i;
			GetMotoPos(pDriverMsg);
			SteeringMotorRightFrontDriverValue = pDriverMsg->RxData.S32;
			SteeringMotorRightFrontEncoderValue=GetSteeringMotorPosition(i-SteeringMotorId);

			SteeringMotorRightFrontEncoderValue/=4;
			SteeringMotorRightFrontDriverValue/=4;
	    
			DebugPrintf("SMRF:%i %i\n", SteeringMotorRightFrontEncoderValue, 
					SteeringMotorRightFrontDriverValue);
		}
		vTaskDelayUntil( &xLastWakeTime, 300 / portTICK_RATE_MS );
	}
}