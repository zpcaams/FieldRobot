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

#include "robot_common.h"

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
