/*****************************************************************************/
/**
* @file robot_led.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "robot_led.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
xQueueHandle xLedQueue;

void LedInitialise(void)
{
	Led_TypeDef Led;
	for(Led=LED1;Led<LEDn;Led++){
		STM_EVAL_LEDInit(Led);
		STM_EVAL_LEDOff(Led);
	}
	xLedQueue = xQueueCreate( 4, sizeof(Dir_TypeDef) );
	xTaskCreate( LedFlashTask, ( signed char * ) "Led",
			configMINIMAL_STACK_SIZE, NULL, LedFlash_TASK_PRIORITY, NULL );
}

void LedFlashTask(void *pvParameters)
{
	Dir_TypeDef Dir = DirAll;
    portTickType xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();
    for(;;){
    	xQueueReceive(xLedQueue, &Dir, 1/portTICK_RATE_MS);
    	switch(Dir){
    	case LeftFront:
    		STM_EVAL_LEDToggle(LED1);
    		STM_EVAL_LEDToggle(LED2);
    		STM_EVAL_LEDOff(LED3);
    		STM_EVAL_LEDOff(LED4);
    		break;
    	case RightFront:
    		STM_EVAL_LEDToggle(LED2);
    		STM_EVAL_LEDToggle(LED3);
    		STM_EVAL_LEDOff(LED1);
    		STM_EVAL_LEDOff(LED4);
    		break;
    	case RightBack:
    		STM_EVAL_LEDToggle(LED3);
    		STM_EVAL_LEDToggle(LED4);
    		STM_EVAL_LEDOff(LED1);
    		STM_EVAL_LEDOff(LED2);
    		break;
    	case LeftBack:
    		STM_EVAL_LEDToggle(LED1);
    		STM_EVAL_LEDToggle(LED4);
    		STM_EVAL_LEDOff(LED2);
    		STM_EVAL_LEDOff(LED3);
    		break;
    	default:
    		STM_EVAL_LEDToggle(LED1);
    		STM_EVAL_LEDToggle(LED2);
    		STM_EVAL_LEDToggle(LED3);
    		STM_EVAL_LEDToggle(LED4);
    		break;
    	}
		vTaskDelayUntil( &xLastWakeTime, 500/portTICK_RATE_MS );
    }
}