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
#define WheelMotorId	4
#define SteeringMotorId	8
#define PutterId 		12
   
/**************************** Type Definitions *******************************/

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
	u8 	Id;			/* Driver Id */
	u8 	Len;		/* CAN Msg Length */
	u8 	Cmd;		/* Msg Command */
	u8 	TxData[4];	/* TxData */
	u8 	RxData[4];	/* RxData */
} DriverMsg_TypeDef;
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void DriverSelfTest(void);

void SetMotoSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotoSpeed (DriverMsg_TypeDef *DriverMsg);
void GetMotoCurrent (DriverMsg_TypeDef *DriverMsg);
void GetMotoTemp (DriverMsg_TypeDef *DriverMsg);
void GetMotoError (DriverMsg_TypeDef *DriverMsg);
void SetMotoPos (DriverMsg_TypeDef *DriverMsg);
void GetMotoPos (DriverMsg_TypeDef *DriverMsg);
void GetMotoType (DriverMsg_TypeDef *DriverMsg);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
