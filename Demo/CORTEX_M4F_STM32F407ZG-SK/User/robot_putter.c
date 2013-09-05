/*****************************************************************************/
/**
* @file robot_putter.c
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "robot_putter.h"

/************************** Constant Definitions *****************************/

#define	RobotHeight_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotHeightStop_TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern	u16	ADCx_Buffer[ADCx_BUFFER_SIZE];
extern	u8	RobotStatus;
/************************** Private Functions ********************************/

void EnterRobotHeightStatus (void)
{
	RobotStatus = ROBOT_HEIGHT;
	xTaskCreate( RobotHeightTask, ( signed char * ) "RbtHt",
			configMINIMAL_STACK_SIZE, NULL, RobotHeight_TASK_PRIORITY, NULL );
}
void EnterRobotHeightStopStatus (void)
{
	RobotStatus = ROBOT_HEIGHT_STOP;
}

void RobotHeightTask (void *pvParameters)
{
	Dir_TypeDef  Dir;
	s16 PWM_SetUp;
	u16 PosSetUp;
	s16 PWM[DirMax];
	u32 Position[DirMax];
	u8 j;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* calculate the PWM here */
    	PWM_SetUp = (GetRemoteControl(2-1)-1440)/4;
		if((PWM_SetUp<50)&&(PWM_SetUp>-50)){
			PWM_SetUp = 0;
		}else if(PWM_SetUp>100){
			PWM_SetUp = 100;
		}else if(PWM_SetUp<-100){
			PWM_SetUp = -100;
		}
//		DebugPrintf("%i\n", PWM);

	    /* 
	     * Calculate the Position Setup here
	     * 0-840
	     */
		PosSetUp = (GetRemoteControl(5-1)-1040)+300;
		
	    /* Calculate the Position here */
		for(Dir=DirMin;Dir<DirMax;Dir++){
			Position[Dir] = 0;
			for(j=0;j<AVERAGE_NUM;j++){
				Position[Dir] += ADCx_Buffer[Dir+j*DirMax];
			}
			Position[Dir] = Position[Dir] /(AVERAGE_NUM);
			
			PWM[Dir] = PWM_SetUp;

			/*
			 * Change Position by SetUp
			 */
			if((Position[Dir]<(PosSetUp-0))&&(PWM_SetUp<0)){
				PWM[Dir] = 0;
			}else if((Position[Dir]>(PosSetUp-10))&&(PWM_SetUp>0)){
				PWM[Dir] = 0;
			}
			
			/*
			 * Position Limitation
			 */			
			if((Position[Dir]<300)&&(PWM_SetUp<0)){
				PWM[Dir] = 0;
			}else if((Position[Dir]>900)&&(PWM_SetUp>0)){
				PWM[Dir] = 0;
			}
		}
		
	    /* Send Command to Driver */	
		pDriverMsg->Base = EP_BASE;
		for(Dir=DirMin;Dir<DirMax;Dir++){
			pDriverMsg->Dir = Dir;
			pDriverMsg->TxData.S16 = PWM[Dir];
			SetMotorPWM (pDriverMsg);			
		}
		
//		DebugPrintf("RobotHeightTask is running\n");
		
        if(RobotStatus==ROBOT_HEIGHT_STOP){
			xTaskCreate( RobotHeightStopTask, ( signed char * ) "RbtHtStp",
					configMINIMAL_STACK_SIZE, NULL, RobotHeightStop_TASK_PRIORITY, NULL );
			vTaskDelete(NULL);
        }
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    }
}

void RobotHeightStopTask (void *pvParameters)
{
	Dir_TypeDef  Dir;
	s16 PWM;
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* calculate the PWM here */
		PWM = 0;
		
	    /* Send Command to Driver */	
		pDriverMsg->Base = EP_BASE;
		for(Dir=DirMin;Dir<DirMax;Dir++){
			pDriverMsg->Dir = Dir;
			pDriverMsg->TxData.S16 = PWM;
			SetMotorPWM (pDriverMsg);			
		}    
		
//		DebugPrintf("RobotHeightStopTask is running\n");
		ResetRoborBusy();
		vTaskDelete(NULL);
        
		vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    
    }   
}

void SetUpPutterDriver(void)
{
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
	Dir_TypeDef Dir;
	u16 NewId;

	for(Dir=DirMin;Dir<DirMax;Dir++){
		NewId = EP_BASE+Dir;
		pDriverMsg->Base = DEFUALT_BASE;
		pDriverMsg->Dir = DirMin;
		pDriverMsg->TxData.U16 = NewId;
		
		SetDriverAdr(pDriverMsg);
		GetDriverAdr(pDriverMsg);
		if(pDriverMsg->RxData.U16==NewId){
			DebugPrintf("Dir %i Address Set to %i\n", Dir, NewId);
		}

		pDriverMsg->Base = EP_BASE;
		pDriverMsg->Dir = Dir;
		pDriverMsg->TxData.S32 = 0x10000+0+768+0;
		
		SetDriverMode(pDriverMsg);
		GetDriverMode(pDriverMsg);
		if(pDriverMsg->RxData.U16==768){
			DebugPrintf("Mode SetUp Done!\n");
		}
		
		
	}
}