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

/* Steering Motor Encoder Adjustment*/
#define SMLF_ADJ    0
#define SMRF_ADJ    (-688)
#define SMRB_ADJ    0
#define SMLB_ADJ    0

/* Couplings Encoder Adjustment*/
#define CPLF_ADJ    0
#define CPRF_ADJ    0
#define CPRB_ADJ    0
#define CPLB_ADJ    0

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

static xSemaphoreHandle xRobotMainSemaphore = NULL;

WheelMotor_4TypeDef      WheelMotor;
SteeringMotor_4TypeDef   SteeringMotor;
static ElectricPutter_4TypeDef  ElectricPutter;
static Couplings_4TypeDef       Couplings;

const uint16_t					RemoteControlDefault[8] ={
		1462, 1439, 1439, 1466, 1062, 1411, 1062, 1047};
/* 2.4G ң�� */
static uint16_t                 RemoteControl[8];

/* ������״̬ */
static u32						RobotMainStatus;

static u16 						AbsEncoderInt[4];

xSemaphoreHandle GetRobotMainSemaphore(void)
{
	return xRobotMainSemaphore;
}

s32 CnvtAbsEncoderVal(u16 Value, s16 Adj)
{
	s32 temp;
	temp = Value + Adj;
	if(temp<-512){
		temp+=1024;
	}else if(temp>511){
		temp-=1024;
	}
	temp*=4;	//�ı�����
	return temp;
}
/*****************************************************************************/
/**
*
* SetSteeringMotorPosition
*
* @param  	Pos
* @param	Value
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetSteeringMotorPosition(u8 Pos, u16 Value)
{
	s32 temp;
	
    switch (Pos){
        case PosLeftFront:{
            SteeringMotor.LeftFront.GP = CnvtAbsEncoderVal(Value, SMLF_ADJ);
            break;
        }
        case PosRightFront: {
        	temp = CnvtAbsEncoderVal(Value, SMRF_ADJ);
            SteeringMotor.RightFront.GP = temp;
//			DebugPrintf("SMRF:%i\n", temp);
            break;
        }        
        case PosRightBack: {
            SteeringMotor.RightBack.GP = CnvtAbsEncoderVal(Value, SMRB_ADJ);
            break;
        }        
        case PosLeftBack: {
            SteeringMotor.LeftBack.GP = CnvtAbsEncoderVal(Value, SMLB_ADJ);
            break;
        }
        default:{
            break;
        }
    }
}

s32 GetSteeringMotorPosition(u8 Pos)
{
    switch (Pos){
        case PosLeftFront:{
            return SteeringMotor.LeftFront.GP;
        }
        case PosRightFront: {
        	return SteeringMotor.RightFront.GP;
        }        
        case PosRightBack: {
        	return SteeringMotor.RightBack.GP;
        }        
        case PosLeftBack: {
        	return SteeringMotor.LeftBack.GP;
        }
        default:{
        	return 0;
        }
    }
}
/*****************************************************************************/
/**
*
* SetCouplingsPosition
*
* @param  	Pos
* @param	Value
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetCouplingsPosition(uint8_t Pos, uint16_t Value)
{
    switch (Pos){
        case PosLeftFront:{
            Couplings.LeftFront.GP = Value+CPLF_ADJ;
            break;
        }
        case PosRightFront: {
            Couplings.RightFront.GP = Value+CPRF_ADJ;
            break;
        }        
        case PosRightBack: {
            Couplings.RightBack.GP = Value+CPRB_ADJ;
            break;
        }        
        case PosLeftBack: {
            Couplings.LeftBack.GP = Value+CPLB_ADJ;
            break;
        }
        default:{
            break;
        }
    }
}

/*****************************************************************************/
/**
*
* SetRemoteControl
*
* @param  	Channel	0~7
* @param	Data
*
* @return	None
*
* @note		None
*
******************************************************************************/
void SetRemoteControl(uint8_t Channel, uint16_t Data)
{
	if((Data>1000)&&(Data<2000)){
		RemoteControl[Channel] = Data;  
	}else{
		RemoteControl[Channel] = RemoteControlDefault[Channel];
	}
}

/*****************************************************************************/
/**
*
* GetRemoteControl
*
* @param  	Channel	0~7
*
* @return	Remote Control Value
*
* @note		None
*
******************************************************************************/
uint16_t GetRemoteControl(uint8_t Channel)
{
	return RemoteControl[Channel];
}

void SetAbsEncoderInt(uint8_t Channel, uint16_t Data)
{
	AbsEncoderInt[Channel] = Data;  
}

u16 GetAbsEncoderInt(uint8_t Channel)
{
	return AbsEncoderInt[Channel];
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
* 			���У�ת��������ǰ����챵���ٶ�Ϊ��
* 			ֱ�У�ת��������ǰ����챵������ͬ�ٶ�����
* 			б�У�ת��������ͬһ�Ƕȣ���챵������ͬ�ٶ�����
* 			ת�䣺ת����
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
    	SPISelfTest();
    	DebugPrintf("Create Encoder Refersh Task\n");
    	xTaskCreate( EncoderRefershTask, ( signed char * ) "Encoder",
    			configMINIMAL_STACK_SIZE, NULL, Encoder_TASK_PRIORITY, NULL );
    	
    	CANSelfTest();    	
    	xTaskCreate( SteeringMotorPosInitializeTask, ( signed char * ) "PosInit",
    			configMINIMAL_STACK_SIZE, NULL, PositionInitialize_TASK_PRIORITY, NULL );
    	
    	do{
    		xStatus = xSemaphoreTake(xRobotMainSemaphore, 100/portTICK_RATE_MS);
    	}while(xStatus==pdTRUE);
				
		xTaskCreate( CANMainTask, ( signed char * ) "CanMain",
				configMINIMAL_STACK_SIZE, NULL, CANMain_TASK_PRIORITY, NULL );

//		xTaskCreate( SteeringMotorPosTestTask, ( signed char * ) "SMT",
//				configMINIMAL_STACK_SIZE, NULL, CANMain_TASK_PRIORITY, NULL );

//		DebugPrintf("SteeringMotorPosTestTask Created.\n");
    	do{
    		xStatus = xSemaphoreTake(xRobotMainSemaphore, portMAX_DELAY);
    	}while(xStatus==pdTRUE);
    	
        vTaskDelayUntil( &xLastWakeTime, 10 / portTICK_RATE_MS );
    }
}

void GreatRobotMainTask(void)
{
	vSemaphoreCreateBinary(xRobotMainSemaphore);

	xTaskCreate( RobotMainTask, ( signed char * ) "RobotMain",
		  configMINIMAL_STACK_SIZE, NULL, RobotMain_TASK_PRIORITY, NULL );
	
}
