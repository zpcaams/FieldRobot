/*****************************************************************************/
/**
*
* @file robot_move.h
*
*
******************************************************************************/

#ifndef ROBOT_MOVE_H
#define ROBOT_MOVE_H

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

/* MLDS includes. */
#include "mlds_can.h"
 
#include "robot_status.h"
#include "robot_driver.h"

/************************** Constant Definitions *****************************/
	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void EnterRobotMoveStatus (void);
void EnterRobotMoveStopStatus (void);
void RobotMoveTask (void *pvParameters);
void RobotMoveStopTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_MOVE_H */
