/*****************************************************************************/
/**
*
* @file robot_remote_control.h
*
*
******************************************************************************/

#ifndef ROBOT_REMOTE_CONTROL_H
#define ROBOT_REMOTE_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
#include "robot_spi.h"
 
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

u16 GetRemoteControl(u8 Channel);
 
#ifdef __cplusplus
}
#endif

#endif /* ROBOT_REMOTE_CONTROL_H */
