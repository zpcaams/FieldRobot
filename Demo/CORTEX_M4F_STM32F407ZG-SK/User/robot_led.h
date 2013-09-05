/*****************************************************************************/
/**
*
* @file robot_led.h
*
*
******************************************************************************/

#ifndef ROBOT_LED_H
#define ROBOT_LED_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
#include "stm32f4_discovery.h"
	
/************************** Constant Definitions *****************************/

	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void LedInitialise(void);
void LedFlashTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_LED_H */
