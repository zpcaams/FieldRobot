/*****************************************************************************/
/**
* @file robot_status.c
*
*
* This file contains all robot status include the status from sensors, moto
* drivers and higher level status.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_status.h"

/************************** Constant Definitions *****************************/

#define MainSwitchOff	(GetRemoteControl(8-1)<1439)
#define ModeSellect		(GetRemoteControl(7-1))
#define MODE_MOVE		1100
#define MODE_TURN		1200
#define MODE_ROLL		1300
#define MODE_HEIGHT		1400
#define MODE_WIDTH		1500
#define MODE_TEST		1600

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

xSemaphoreHandle RobotStatusSemaphore;

/* 机器人状态 */
u8						RobotStatus;
static u8				RobotBusy;


void ResetRoborBusy(void)
{
	RobotBusy = 0;
}

/*****************************************************************************/
/**
*
* Robot Main Task
*
* @param  	None
*
* @return	None
*
* @note		Before this task run, every periphers, queues and send/receive tasks
* 			should be initialized.
* 			Working flow:
* 			Run SPI selftest
* 			Start Encoder Task
* 			Run CAN selftest
* 			Initialize Steering Motor Position
* 			System Cal/Init Done, goto Running Status.
* 			
* 			In Running Status:
* 			空闲：转向电机均朝前，轮毂电机速度为零
* 			直行：转向电机均朝前，轮毂电机以相同速度运行
* 			斜行：转向电机均朝同一角度，轮毂电机以相同速度运行
* 			转弯：转向电机
* 			
* 			
*
******************************************************************************/
void RobotMainTask (void *pvParameters)
{
    portBASE_TYPE xStatus;
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    for( ; ; )
    {
    	if(RobotStatus==ROBOT_INIT){
        	SPISelfTest();
        	DebugPrintf("SPI SelfTest Done!\n");
        	DebugPrintf("Create Encoder Refersh Task\n");
        	xTaskCreate( SpiRefershTask, ( signed char * ) "Encoder",
        			configMINIMAL_STACK_SIZE, NULL, Encoder_TASK_PRIORITY, NULL );

        	xTaskCreate( SteeringMotorPosInitTask, ( signed char * ) "PosInit",
        			configMINIMAL_STACK_SIZE, NULL, PositionInit_TASK_PRIORITY, NULL );
        	
        	do{
        		xStatus = xSemaphoreTake(RobotStatusSemaphore, 100/portTICK_RATE_MS);
        	}while(xStatus==pdTRUE);

//        	SetUpPutterDriver();

//        	DebugPrintf("Create ADC Refersh Task\n");
//        	xTaskCreate( AdcRefershTask, ( signed char * ) "PosInit",
//        			configMINIMAL_STACK_SIZE, NULL, AdcRefersh_TASK_PRIORITY, NULL );
        	
        	DebugPrintf("Robot Initialize Done!\n");
        	RobotStatus = ROBOT_IDLE;
    		
    	}else{
    		switch(RobotStatus){
    		
    		case ROBOT_IDLE:
    			
				if(MainSwitchOff){
						DebugPrintf("MODE_IDLE ");
					if(ModeSellect<MODE_MOVE){
						DebugPrintf("Will Switch to MODE_MOVE\n");
					}else if(ModeSellect<MODE_TURN){
						DebugPrintf("Will Switch to MODE_TURN\n");
					}else if(ModeSellect<MODE_ROLL){
						DebugPrintf("Will Switch to MODE_ROLL\n");
					}else if(ModeSellect<MODE_HEIGHT){
						DebugPrintf("Will Switch to MODE_HEIGHT\n");
					}else if(ModeSellect<MODE_WIDTH){
						DebugPrintf("Will Switch to MODE_WIDTH\n");
					}else{
						DebugPrintf("Will Switch to MODE_TEST\n");
					}
				}else{
					RobotBusy = 1;
					if(ModeSellect<MODE_MOVE){
						EnterRobotMoveStatus();
						DebugPrintf("Switch to MODE_MOVE\n");
					}else if(ModeSellect<MODE_TURN){
						RobotStatus = ROBOT_TURN;
						DebugPrintf("Switch to MODE_TURN\n");
					}else if(ModeSellect<MODE_ROLL){
						RobotStatus = ROBOT_ROLL;
						DebugPrintf("Switch to MODE_ROLL\n");
					}else if(ModeSellect<MODE_HEIGHT){
						EnterRobotHeightStatus();
						DebugPrintf("Switch to MODE_HEIGHT\n");
					}else if(ModeSellect<MODE_HEIGHT){
						RobotStatus = ROBOT_WIDTH;
						DebugPrintf("Switch to ROBOT_WIDTH\n");
					}else{
						EnterRobotTestStatus();
						DebugPrintf("Switch to MODE_TEST\n");
					}
				}
    			break;
    		
    		case ROBOT_MOVE:
    			
				if(MainSwitchOff){
					EnterRobotMoveStopStatus();
					DebugPrintf("Switch to ROBOT_MOVE_STOP\n");
				}
    			break;
    			
    		case ROBOT_MOVE_STOP:
    			
				if(RobotBusy==0){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		case ROBOT_TURN:
    			
				if(MainSwitchOff){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		case ROBOT_ROLL:
    			
				if(MainSwitchOff){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		case ROBOT_HEIGHT:
    			
				if(MainSwitchOff){
					EnterRobotHeightStopStatus();
					DebugPrintf("Switch to ROBOT_HEIGHT_STOP\n");
				}
    			break;
    			
    		case ROBOT_HEIGHT_STOP:
    			
				if(RobotBusy==0){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		case ROBOT_WIDTH:
    			
				if(MainSwitchOff){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		case ROBOT_TEST:
			
				if(MainSwitchOff){
					EnterRobotTestStopStatus();
					DebugPrintf("Switch to ROBOT_TEST_STOP\n");
				}
				break;
    			
    		case ROBOT_TEST_STOP:
    			
				if(RobotBusy==0){
					RobotStatus = ROBOT_IDLE;
					DebugPrintf("Switch to ROBOT_IDLE\n");
				}
    			break;
    			
    		default:
					RobotStatus = ROBOT_IDLE;
    			break;
    		}
    	}
        vTaskDelayUntil( &xLastWakeTime, 1000 / portTICK_RATE_MS );
    }
}

void GreatRobotMainTask(void)
{
	RobotStatus = ROBOT_INIT;
	vSemaphoreCreateBinary(RobotStatusSemaphore);
	xTaskCreate( RobotMainTask, ( signed char * ) "RobotMain",
		  configMINIMAL_STACK_SIZE, NULL, RobotMain_TASK_PRIORITY, NULL );
	
}
