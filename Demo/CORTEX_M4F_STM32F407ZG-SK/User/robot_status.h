/*****************************************************************************/
/**
*
* @file robot_debug.h
*
*
******************************************************************************/

#ifndef __ROBOT_STATUS_H
#define __ROBOT_STATUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

 /* ST Driver includes. */
 #include "stm32f4xx_conf.h"
 
/************************** Constant Definitions *****************************/
 
/* Position Define */
#define PosLeftFront      0
#define PosRightFront     1
#define PosRightBack      2
#define PosLeftBack       3
 
/**************************** Type Definitions *******************************/
 
/* 轮毂电机 */
typedef struct
{
  int32_t V;
  int32_t GV;
  int16_t GC;
  int16_t GT;
  uint16_t GEI;
}WheelMotor_TypeDef;

typedef struct
{
  WheelMotor_TypeDef LeftFront;
  WheelMotor_TypeDef RightFront;
  WheelMotor_TypeDef RightBack;
  WheelMotor_TypeDef LeftBack;
}
WheelMotor_4TypeDef;

/* 转向电机 */
typedef struct
{
  int32_t M;
  uint16_t GP;           //位置信息，由SPI接口得到
  int32_t GV;
  int16_t GC;
  int16_t GT;
  uint16_t GEI;
}SteeringMotor_TypeDef;

typedef struct
{
  SteeringMotor_TypeDef LeftFront;
  SteeringMotor_TypeDef RightFront;
  SteeringMotor_TypeDef RightBack;
  SteeringMotor_TypeDef LeftBack;
}
SteeringMotor_4TypeDef;

/* 电动推杆 */
typedef struct
{
  uint32_t EC;
  uint16_t GC; 
  uint16_t GP;          //位置信息，由ADC采集得到
  int16_t GT;
  uint16_t GEI;
}ElectricPutter_TypeDef;

typedef struct
{
  ElectricPutter_TypeDef LeftFront;
  ElectricPutter_TypeDef RightFront;
  ElectricPutter_TypeDef RightBack;
  ElectricPutter_TypeDef LeftBack;
}
ElectricPutter_4TypeDef;

/* 联轴器 */
typedef struct
{
  uint16_t GP;           //位置信息，由SPI接口得到
}
Couplings_TypeDef;

typedef struct
{
  ElectricPutter_TypeDef LeftFront;
  ElectricPutter_TypeDef RightFront;
  ElectricPutter_TypeDef RightBack;
  ElectricPutter_TypeDef LeftBack;
}
Couplings_4TypeDef;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void SetSteeringMotorPosition(uint8_t Po, uint16_t GP);
void SetCouplingsPosition(uint8_t Po, uint16_t GP);
void SetRemoteControl(uint8_t Channel, uint16_t Data);

#ifdef __cplusplus
}
#endif

#endif /* __ROBOT_STATUS_H */