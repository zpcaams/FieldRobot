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
   
#include "robot_can.h"

/************************** Constant Definitions *****************************/\

/**************************** Type Definitions *******************************/

/* Moto Drivers CAN ID Base */
typedef enum 
{
	WM_BASE	= 4,
	SM_BASE	= 8,
	EP_BASE	= 12,
	DEFUALT_BASE = 127
} IdBase_TypeDef;

/* 轮毂电机 */
typedef struct
{
	s32	V;
	s32 GV;
	s16 GC;
	s16 GT;
	u16 GEI;
}WheelMotor_TypeDef;

/* 转向电机 */
typedef struct
{
	s32	M;
	s32 GM;
	s16 GC;
	s16 GT;
	u16 GEI;
}SteeringMotor_TypeDef;

/* 电动推杆 */
typedef struct
{
	s16 AM;
	s16 GC;
	s16 GT;
	u16 GEI;
}ElectricPutter_TypeDef;

typedef struct
{
	IdBase_TypeDef 	Base;		/* Driver Id Base */
	Dir_TypeDef 	Dir;		/* Driection of the Driver */
	u8 				Len;		/* CAN Msg Length */
	u8 				Cmd;		/* Msg Command */
	union{
		u8 			U8[4];
		s32			S32;
		s16			S16;
		u16			U16;
	}TxData;		/* TxData */
	union{
		u8 			U8[4];
		s32			S32;
		s16			S16;
		u16			U16;
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

void SetDriverAdr (DriverMsg_TypeDef *DriverMsg);
void GetDriverAdr (DriverMsg_TypeDef *DriverMsg);
void SaveParameter (DriverMsg_TypeDef *DriverMsg);
void SetDriverMode (DriverMsg_TypeDef *DriverMsg);
void GetDriverMode (DriverMsg_TypeDef *DriverMsg);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
