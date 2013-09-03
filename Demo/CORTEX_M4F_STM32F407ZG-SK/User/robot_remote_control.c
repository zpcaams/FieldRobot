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
	u16 Temp;

	Value = GetSpiBuffer(Channel+8);
	
	if(Channel==(6-1)){
		/* Special Channel 6, 0~150 */
		Temp = (GetSpiBuffer(3+8));
		if((Temp>1190)&&(Temp<1740)){
			return (Value-Temp+75);
		}else{
			return 0;
		}
	}else{
		if((Value>1030)&&(Value<1890)){
			return Value;
		}else{
			return RemoteControlDefault[Channel];
		}
	}
}