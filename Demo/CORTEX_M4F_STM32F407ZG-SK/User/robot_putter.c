/*****************************************************************************/
/**
* @file robot_putter.c
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "robot_putter.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Private Functions ********************************/

void SetUpPutterDriver(void)
{
	DriverMsg_TypeDef DriverMsg;
	DriverMsg_TypeDef *pDriverMsg = &DriverMsg;
	Dir_TypeDef Dir;
	u16 NewId;

	for(Dir=DirMin;Dir<DirMax;Dir++){
		NewId = EP_BASE+Dir;
		pDriverMsg->Base = DEFUALT_BASE;
		pDriverMsg->Dir = DirMin;
		pDriverMsg->TxData.U16 = NewId;
		
		SetDriverAdr(pDriverMsg);
		GetDriverAdr(pDriverMsg);
		if(pDriverMsg->RxData.U16==NewId){
			DebugPrintf("Dir %i Address Set to %i\n", Dir, NewId);
		}

		pDriverMsg->Base = EP_BASE;
		pDriverMsg->Dir = Dir;
		pDriverMsg->TxData.S32 = 0x10000+0+768+0;
		
		SetDriverMode(pDriverMsg);
		GetDriverMode(pDriverMsg);
		if(pDriverMsg->RxData.U16==768){
			DebugPrintf("Mode SetUp Done!\n");
		}
		
		
	}
}