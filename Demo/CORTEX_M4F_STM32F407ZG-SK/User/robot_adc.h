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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"

/* Robot includes */
#include "robot_debug.h"
	
/************************** Constant Definitions *****************************/

 
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void AdcRefershTask( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_ADC_H */
