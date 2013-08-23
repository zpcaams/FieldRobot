/*****************************************************************************/
/**
* @file robot_couplings.c
*
*
* This file contains robot couplings control code.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_couplings.h"

/************************** Constant Definitions *****************************/

#define CPLSn						4

#define	CPLS_LF_PIN					GPIO_Pin_9
#define	CPLS_LF_GPIO_PORT			GPIOE
#define	CPLS_LF_GPIO_CLK			RCC_AHB1Periph_GPIOE

#define	CPLS_RF_PIN					GPIO_Pin_10
#define	CPLS_RF_GPIO_PORT			GPIOE
#define	CPLS_RF_GPIO_CLK			RCC_AHB1Periph_GPIOE

#define	CPLS_RB_PIN					GPIO_Pin_11
#define	CPLS_RB_GPIO_PORT			GPIOE
#define	CPLS_RB_GPIO_CLK			RCC_AHB1Periph_GPIOE

#define	CPLS_LB_PIN					GPIO_Pin_8
#define	CPLS_LB_GPIO_PORT			GPIOE
#define	CPLS_LB_GPIO_CLK			RCC_AHB1Periph_GPIOE


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

GPIO_TypeDef* COUPLINGS_PORT[CPLSn] = {CPLS_LF_GPIO_PORT, CPLS_RF_GPIO_PORT, 
									CPLS_RB_GPIO_PORT, CPLS_LB_GPIO_PORT};
const uint16_t COUPLINGS_PIN[CPLSn] = {CPLS_LF_PIN, CPLS_RF_PIN, CPLS_RB_PIN,
									CPLS_LB_PIN};
const uint32_t COUPLINGS_CLK[CPLSn] = {CPLS_LF_GPIO_CLK, CPLS_RF_GPIO_CLK, 
									CPLS_RB_GPIO_CLK, CPLS_LB_GPIO_CLK};

/************************** Private Functions ********************************/

void SingleCouplingsOn(Pos_TypeDef Position)
{
	COUPLINGS_PORT[Position]->BSRRL = COUPLINGS_PIN[Position];
}

void SingleCouplingsOff(Pos_TypeDef Position)
{
	COUPLINGS_PORT[Position]->BSRRH = COUPLINGS_PIN[Position];  
}

void CouplingsOff(void)
{
	SingleCouplingsOff(PosLeftFront);
	SingleCouplingsOff(PosRightFront);
	SingleCouplingsOff(PosRightBack);
	SingleCouplingsOff(PosLeftBack);
}

void CouplingsOn(Pos_TypeDef Position)
{
	CouplingsOff();
	SingleCouplingsOn(Position);
}

void CouplingsPinInit(Pos_TypeDef Position)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(COUPLINGS_CLK[Position], ENABLE);
	
	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = COUPLINGS_PIN[Position];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COUPLINGS_PORT[Position], &GPIO_InitStructure);
}

void CouplingsInitialise(void)
{
	CouplingsPinInit(PosLeftFront);
	CouplingsPinInit(PosRightFront);
	CouplingsPinInit(PosRightBack);
	CouplingsPinInit(PosLeftBack);
	CouplingsOff();	
}