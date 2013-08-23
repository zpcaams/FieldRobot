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