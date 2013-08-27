/*****************************************************************************/
/**
*
* @file robot_driver.h
*
*
******************************************************************************/

#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"

/* MLDS includes. */
#include "mlds_can.h"

/************************** Constant Definitions *****************************/\
 
/* Moto Drivers CAN ID Base */
#define WheelMotorId		4
#define SteeringMotorId		8
#define ElectricPutterId 	12
   
/**************************** Type Definitions *******************************/

/* ��챵�� */
typedef struct
{
	s32	V;
	s32 GV;
	s16 GC;
	s16 GT;
	u16 GEI;
}WheelMotor_TypeDef;

/* ת���� */
typedef struct
{
	s32	M;
	s32 GM;
	s16 GC;
	s16 GT;
	u16 GEI;
}SteeringMotor_TypeDef;

/* �綯�Ƹ� */
typedef struct
{
	s16 AM;
	s16 GC;
	s16 GT;
	u16 GEI;
}ElectricPutter_TypeDef;

typedef struct
{
	u8 	Id;			/* Driver Id */
	u8 	Len;		/* CAN Msg Length */
	u8 	Cmd;		/* Msg Command */
	union{
		u8 	U8[4];
		s32	S32;
		s16	S16;
	}TxData;		/* TxData */
	union{
		u8 	U8[4];
		s32	S32;
		s16	S16;
		u16	U16;
	}RxData;		/* RxData */
} DriverMsg_TypeDef;
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void DriverSelfTest(void);

void SetMotorSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotorSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotorCurrent (DriverMsg_TypeDef *DriverMsg);
void GetMotorTemp (DriverMsg_TypeDef *DriverMsg);
void GetMotorError (DriverMsg_TypeDef *DriverMsg);
void SetMotorPos (DriverMsg_TypeDef *DriverMsg);
void SetMotorMove (DriverMsg_TypeDef *DriverMsg);
void GetMotorPos (DriverMsg_TypeDef *DriverMsg);
void SetMotorPWM (DriverMsg_TypeDef *DriverMsg);
void GetMotoType (DriverMsg_TypeDef *DriverMsg);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
