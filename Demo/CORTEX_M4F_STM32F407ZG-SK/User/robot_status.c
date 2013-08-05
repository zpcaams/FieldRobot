

/* Includes ------------------------------------------------------------------*/
#include "robot_status.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static WheelMotor_4TypeDef      WheelMotor;
static SteeringMotor_4TypeDef   SteeringMotor;
static ElectricPutter_4TypeDef  ElectricPutter;
static Couplings_4TypeDef       Couplings;

/* 2.4G Ò£¿Ø */
static uint16_t                 RemoteControl[8];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void SetSteeringMotorPosition(uint8_t Po, uint16_t GP)
{
    switch (Po){
        case PoLeftFront:{
            SteeringMotor.LeftFront.GP = GP+SMLF_ADJ;
            break;
        }
        case PoRightFront: {
            SteeringMotor.RightFront.GP = GP+SMRF_ADJ;
            break;
        }        
        case PoRightBack: {
            SteeringMotor.RightBack.GP = GP+SMRB_ADJ;
            break;
        }        
        case PoLeftBack: {
            SteeringMotor.LeftBack.GP = GP+SMLB_ADJ;
            break;
        }
        default:{
            break;
        }
    }
}

void SetCouplingsPosition(uint8_t Po, uint16_t GP)
{
    switch (Po){
        case PoLeftFront:{
            Couplings.LeftFront.GP = GP+CPLF_ADJ;
            break;
        }
        case PoRightFront: {
            Couplings.RightFront.GP = GP+CPRF_ADJ;
            break;
        }        
        case PoRightBack: {
            Couplings.RightBack.GP = GP+CPRB_ADJ;
            break;
        }        
        case PoLeftBack: {
            Couplings.LeftBack.GP = GP+CPLB_ADJ;
            break;
        }
        default:{
            break;
        }
    }
}

void SetRemoteControl(uint8_t Channel, uint16_t Data)
{
    if ((Channel>=1)||(Channel<=8)){
        RemoteControl[Channel] = Data;    
    }
}