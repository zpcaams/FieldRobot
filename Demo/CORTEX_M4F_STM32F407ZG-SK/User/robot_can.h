/*****************************************************************************/
/**
*
* @file robot_can.h
*
*
******************************************************************************/

#ifndef ROBOT_CAN_H
#define ROBOT_CAN_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"

/************************** Constant Definitions *****************************/

#define CANMsgSend_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 4UL )

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void CANInitialise(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_DRIVER_H */
