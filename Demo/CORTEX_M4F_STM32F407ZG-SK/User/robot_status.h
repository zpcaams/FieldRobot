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
 * ������״̬��˵��
 * 
 * ��ʼ������ʼ������ģ�����
 * ���У����������Ĭ��λ�ã��ٶ�Ϊ��
 * ƽ������������ͬ�Ƕȣ���ͬ�ٶ��ƶ�
 * ת�䣺���ո���ת��뾶����
 * ��ת��ԭ����ת��ת
 * �߶ȵ��ڣ��Ƹ˵��ڳ���
 * �־���ڣ����Ȼ�Բ��--ֻ��һ�ȶ�
 * 
 * ��ʼ����״̬��
 * �ϵ磺������Ĭ��״̬
 * �Լ죺������ģ���Ƿ�������
 * �������Լ���ϣ����Խ���ָ�״̬תΪ����
 * 
 * ������״̬��
 * ң�����ܿ��ش򿪣�����ģʽѡ��
 * ĳһ����״̬�£�
 * ң�����ܿ��عرգ�ֹͣ������������
 * ֹͣ״̬��
 * �ȴ����ֹͣ��ת�󣬻ص�����״̬
 * 
 * ͨ��	ң����λ��		��������	ƽ��	ת��	��ת	�߶�	�־�
 * ͨ��1	����ҡ������	ת��		O	O	X	X	X
 * ͨ��3	����ҡ������	�ٶ�		O	O	O	X	X
 * ͨ��5	��ǰ��ť		�߶�		X	X	X	O	X
 * ͨ��6	�� ��ť		�־�		X	X	X	X	O
 * ͨ��7	��ǰ��ť		ģʽѡ��	X	X	X	X	X
 * ͨ��8	��ǰ�Ͽ���           	�ܿ���
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
 
/* ��챵�� */
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

/* ת���� */
typedef struct
{
  int32_t M;
  s32 GP;           //λ����Ϣ����SPI�ӿڵõ� ,ȡֵ��Χ-512~511
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

/* �綯�Ƹ� */
typedef struct
{
  uint32_t EC;
  uint16_t GC; 
  uint16_t GP;          //λ����Ϣ����ADC�ɼ��õ�
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

/* ������ */
typedef struct
{
  uint16_t GP;           //λ����Ϣ����SPI�ӿڵõ�
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