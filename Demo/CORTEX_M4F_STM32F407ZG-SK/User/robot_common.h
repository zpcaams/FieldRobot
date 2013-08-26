/*****************************************************************************/
/**
*
* @file robot_common.h
*
*
******************************************************************************/

#ifndef ROBOT_COMMON_H
#define ROBOT_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Debug Printf. */
#include "robot_debug.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"
 
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

 /* Direction Define */
 typedef enum 
 {
 	LeftFront	= 0,
 	RightFront	= 1,
 	RightBack	= 2,
 	LeftBack	= 3,
 	DirMax		= 4
 } Dir_TypeDef;
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_COMMON_H */
