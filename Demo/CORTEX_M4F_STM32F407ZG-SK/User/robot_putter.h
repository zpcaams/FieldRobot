/*****************************************************************************/
/**
*
* @file robot_putter.h
*
*
******************************************************************************/

#ifndef ROBOT_PUTTER_H
#define ROBOT_PUTTER_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
#include "robot_status.h"
#include "robot_driver.h"
#include "robot_adc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void EnterRobotHeightStatus (void);
void EnterRobotHeightStopStatus (void);
void RobotHeightTask (void *pvParameters);
void RobotHeightStopTask (void *pvParameters);

void SetUpPutterDriver(void);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_PUTTER_H */
