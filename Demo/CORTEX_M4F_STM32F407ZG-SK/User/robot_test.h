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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
 
#include "robot_status.h"
#include "robot_couplings.h"
	
/************************** Constant Definitions *****************************/
	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void EnterRobotTestStatus (void);
void EnterRobotTestStopStatus (void);
void RobotTestTask (void *pvParameters);
void RobotTestStopTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* ROBOT_TEST_H */
