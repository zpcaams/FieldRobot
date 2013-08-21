/*****************************************************************************/
/**
*
* @file robot_couplings.h
*
*
******************************************************************************/

#ifndef ROBOT_COUPLINGS_H
#define ROBOT_COUPLINGS_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_status.h"

/************************** Constant Definitions *****************************/
	
/**************************** Type Definitions *******************************/
	
/* Position Define */
typedef enum 
{
	PosLeftFront	= 0,
	PosRightFront	= 1,
	PosRightBack	= 2,
	PosLeftBack		= 3
} Pos_TypeDef;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void CouplingsInitialise(void);
void CouplingsOn(Pos_TypeDef Position);
void CouplingsOff(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_COUPLINGS_H */
