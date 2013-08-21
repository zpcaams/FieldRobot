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
extern WheelMotor_4TypeDef		WheelMotor;
extern SteeringMotor_4TypeDef	SteeringMotor;
extern u16						RemoteControl[8];

#define CouplingsSellect		(RemoteControl[2-1])

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

    for( ; ; )
    {
    	if(CouplingsSellect<1200){
    		CouplingsOn(PosLeftFront);
    	}else if(CouplingsSellect<1400){
    		CouplingsOn(PosRightFront);
    	}else if(CouplingsSellect>1700){
    		CouplingsOn(PosLeftBack);
    	}else if(CouplingsSellect>1600){
    		CouplingsOn(PosRightBack);
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