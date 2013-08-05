/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ROBOT_STATUS_H
#define __ROBOT_STATUS_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
/* Exported types ------------------------------------------------------------*/
   
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
  uint16_t GP;           //λ����Ϣ����SPI�ӿڵõ�
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


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Position Define */
#define PoLeftFront      0
#define PoRightFront     1
#define PoRightBack      2
#define PoLeftBack       3

/* Exported functions ------------------------------------------------------- */

void SetSteeringMotorPosition(uint8_t Po, uint16_t GP);
void SetCouplingsPosition(uint8_t Po, uint16_t GP);
void SetRemoteControl(uint8_t Channel, uint16_t Data);

#ifdef __cplusplus
}
#endif

#endif /* __ROBOT_STATUS_H */