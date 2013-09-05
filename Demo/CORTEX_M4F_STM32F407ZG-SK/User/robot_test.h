/*****************************************************************************/
/**
*
* @file robot_test.h
*
*
******************************************************************************/

#ifndef ROBOT_TEST_H
#define ROBOT_TEST_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "robot_common.h"
#include "robot_remote_control.h"
#include "robot_status.h"
#include "robot_position.h"
#include "robot_driver.h"
#include "robot_couplings.h"
#include "robot_position.h"
#include "robot_led.h"

/************************** Constant Definitions *****************************/
	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void EnterRobotTestStatus (void);
void EnterRobotTestStopStatus (void);
void RobotTestTask (void *pvParameters);
void RobotTestStopTask (void *pvParameters);
void SteeringMotorPosTestTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_TEST_H */
