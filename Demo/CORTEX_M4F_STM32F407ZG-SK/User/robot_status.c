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
#define SMRF_ADJ    0
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

static WheelMotor_4TypeDef      WheelMotor;
static SteeringMotor_4TypeDef   SteeringMotor;
static ElectricPutter_4TypeDef  ElectricPutter;
static Couplings_4TypeDef       Couplings;

const uint16_t					RemoteControlDefault[8] ={
		1462, 1439, 1439, 1466, 1062, 1411, 1062, 1047};
/* 2.4G Ò£¿Ø */
static uint16_t                 RemoteControl[8];

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
void SetSteeringMotorPosition(uint8_t Pos, uint16_t Value)
{
    switch (Pos){
        case PosLeftFront:{
            SteeringMotor.LeftFront.GP = Value+SMLF_ADJ;
            break;
        }
        case PosRightFront: {
            SteeringMotor.RightFront.GP = Value+SMRF_ADJ;
            break;
        }        
        case PosRightBack: {
            SteeringMotor.RightBack.GP = Value+SMRB_ADJ;
            break;
        }        
        case PosLeftBack: {
            SteeringMotor.LeftBack.GP = Value+SMLB_ADJ;
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