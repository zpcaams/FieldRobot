/*****************************************************************************/
/**
* @file robot_remote_control.C
*
*
* This file contains a driver which get register value of encoder and 
* 2.4G wireless remote control information via spi interface using DMA.
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "robot_remote_control.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

const	static	u16	RemoteControlDefault[8] ={
		1462, 1439, 1439, 1466, 1062, 1411, 1062, 1047};

//	Channel 8~15 	2.4G remote control
u16 GetRemoteControl(u8 Channel)
{
	u16 Value;
	
	Value = GetSpiBuffer(Channel+8);
	
	if((Value>1000)&&(Value<2000)){
		return Value;
	}else{
		return RemoteControlDefault[Channel];
	}
}