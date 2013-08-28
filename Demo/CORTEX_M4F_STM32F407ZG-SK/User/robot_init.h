/*****************************************************************************/
/**
*
* @file robot_init.h
*
*
******************************************************************************/

#ifndef ROBOT_INIT_H
#define ROBOT_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
#include "robot_driver.h"
#include "robot_position.h"

/************************** Constant Definitions *****************************/\
 
/* CAN Task Priority Setup */
#define PositionInit_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 3UL )
#define CANMain_TASK_PRIORITY				        ( tskIDLE_PRIORITY + 3UL )
   
/**************************** Type Definitions *******************************/
 
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void SteeringMotorPosInitTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_INIT_H */
