/*****************************************************************************/
/**
*
* @file robot_adc.h
*
*
******************************************************************************/

#ifndef ROBOT_ADC_H
#define ROBOT_ADC_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
	
/************************** Constant Definitions *****************************/
 
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void AdcInitialise(void);
void AdcRefershTask( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_ADC_H */
