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
#define TEST_ENCODER
#define TEST_COUPLINGS
#define TEST_POSITION
#define TEST_LED
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern u8						RobotStatus;
extern u16						RemoteControl[8];
#ifdef TEST_LED
extern	xQueueHandle xLedQueue;
#endif

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
    
#ifdef TEST_COUPLINGS
    u8 temp;
#endif
#ifdef TEST_LED
    u8 temp;
	Dir_TypeDef Dir;
#endif
    
#ifdef TEST_ENCODER
    u8 i;
    u16	Encoder[8];
#endif

#ifdef TEST_POSITION
	Dir_TypeDef  Dir;
	s32 SMPos;
#endif
	
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
#ifdef TEST_COUPLINGS
		temp = GetRemoteControl(6-1);
		
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
#endif

#ifdef TEST_ENCODER
    	for(i=0;i<8;i++){
    		Encoder[i] = GetSpiBuffer(i);
    		DebugPrintf("%i %i\n", i, Encoder[i]);
    	}
		DebugPrintf("\n", i, Encoder[i]);
#endif
#ifdef TEST_POSITION
		for(Dir=DirMin;Dir<DirMax;Dir++){
			SMPos = GetSterringMotorPosition(Dir);
			DebugPrintf("%i %i\n", Dir, SMPos);
		}
		DebugPrintf("\n", i, Encoder[i]);
#endif

#ifdef TEST_LED
		temp = GetRemoteControl(6-1);
		
    	if(temp<25){
    		Dir = DirAll;
		}else if(temp<50){
    		Dir = LeftFront;
    	}else if(temp<75){
    		Dir = RightFront;
    	}else if(temp<100){
    		Dir = RightBack;
    	}else if(temp<125){
    		Dir = LeftBack;
    	}else{
    		Dir = DirAll;
    	}
		DebugPrintf("Dir %i\n", Dir);
		xQueueSendToBack( xLedQueue, (void *)&Dir, ( portTickType ) 0 );
#endif
        if(RobotStatus==ROBOT_TEST_STOP){
			xTaskCreate( RobotTestStopTask, ( signed char * ) "RbtTstStp",
					configMINIMAL_STACK_SIZE, NULL, RobotTestStop_TASK_PRIORITY, NULL );
			vTaskDelete(NULL);
        }
		vTaskDelayUntil( &xLastWakeTime, 1000 / portTICK_RATE_MS );
    
    }   
}

void RobotTestStopTask (void *pvParameters)
{
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
#ifdef TEST_COUPLINGS
		CouplingsOff();
#endif
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
	Dir_TypeDef Dir;
    portTickType xLastWakeTime;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
	s32 SteeringMotorRightFrontEncoderValue;
	s32 SteeringMotorRightFrontDriverValue;

    xLastWakeTime = xTaskGetTickCount();
    
	for(;;){

		pDriverMsg->Base = SM_BASE;
		for(Dir=DirMin;Dir<DirMax;Dir++){
			
			pDriverMsg->Dir = Dir;
			GetMotorPos(pDriverMsg);
			SteeringMotorRightFrontDriverValue = pDriverMsg->RxData.S32;
			SteeringMotorRightFrontEncoderValue = GetSteeringMotorPosition(Dir);

			SteeringMotorRightFrontEncoderValue/=4;
			SteeringMotorRightFrontDriverValue/=4;
	    
			DebugPrintf("SMRF:%i %i\n", SteeringMotorRightFrontEncoderValue, 
					SteeringMotorRightFrontDriverValue);
		}
		vTaskDelayUntil( &xLastWakeTime, 300 / portTICK_RATE_MS );
	}
}