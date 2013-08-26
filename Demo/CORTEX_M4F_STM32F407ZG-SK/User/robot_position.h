/*****************************************************************************/
/**
*
* @file robot_position.h
*
*
******************************************************************************/

#ifndef ROBOT_POSITION_H
#define ROBOT_POSITION_H

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

s32 GetSterringMotorPosition(Dir_TypeDef Direction);
s32 GetCouplingsPosition(Dir_TypeDef Direction);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_POSITION_H */
