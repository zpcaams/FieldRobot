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
 
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"
 
/* Robot includes */
#include "robot_encoder.h"
#include "robot_driver.h"
#include "robot_move.h"
 
/************************** Constant Definitions *****************************/
 
/* Position Define */
#define PosLeftFront      0
#define PosRightFront     1
#define PosRightBack      2
#define PosLeftBack       3

/* 
 * 机器人状态及说明
 * 
 * 初始化：初始化各个模块参数
 * 空闲：各个电机在默认位置，速度为零
 * 平动：各轮以相同角度，相同速度移动
 * 转弯：按照给定转弯半径行走
 * 回转：原地正转或反转
 * 高度调节：推杆调节长度
 * 轮距调节：单腿划圆弧--只可一腿动
 * 
 * 初始化下状态：
 * 上电：启动后默认状态
 * 自检：检查各个模块是否工作正常
 * 工作：自检完毕，可以接收指令，状态转为空闲
 * 
 * 空闲下状态：
 * 遥控器总开关打开，根据模式选择
 * 某一工作状态下：
 * 遥控器总开关关闭，停止驱动器工作，
 * 停止状态：
 * 等待电机停止运转后，回到空闲状态
 * 
 * 通道	遥控器位置		基本功能	平动	转弯	回转	高度	轮距
 * 通道1	右手摇杆左右	转角		O	O	X	X	X
 * 通道3	右手摇杆下上	速度		O	O	O	X	X
 * 通道5	左前旋钮		高度		X	X	X	O	X
 * 通道6	左 旋钮		轮距		X	X	X	X	O
 * 通道7	右前旋钮		模式选择	X	X	X	X	X
 * 通道8	右前上开关           	总开关
 */
#define	ROBOT_INIT			0
#define	ROBOT_IDLE			1
#define	ROBOT_MOVE			2
#define	ROBOT_MOVE_STOP		3
#define	ROBOT_TURN			4
#define	ROBOT_TURN_STOP		5
#define	ROBOT_ROLL			6
#define	ROBOT_ROLL_STOP		7
#define	ROBOT_HEIGHT		8
#define	ROBOT_HEIGHT_STOP	9
#define	ROBOT_WIDTH			10
#define	ROBOT_WIDTH_STOP	11

#define	RobotMain_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotMove_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotMoveStop_TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )
 
 
 
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
  s32 GP;           //位置信息，由SPI接口得到 ,取值范围-512~511
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

void GreatRobotMainTask(void); 
void SetSteeringMotorPosition(uint8_t Po, uint16_t GP);
s32 GetSteeringMotorPosition(u8 Pos);
void SetCouplingsPosition(uint8_t Po, uint16_t GP);
void SetRemoteControl(uint8_t Channel, uint16_t Data);
uint16_t GetRemoteControl(uint8_t Channel);
void SetAbsEncoderInt(uint8_t Channel, uint16_t Data);
u16 GetAbsEncoderInt(uint8_t Channel);
void ResetRoborBusy(void);

#ifdef __cplusplus
}
#endif

#endif /* __ROBOT_STATUS_H */