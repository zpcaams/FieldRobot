/*****************************************************************************/
/**
* @file robot_position.c
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "robot_position.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

const static s16 SterringMotorPosAdj[DirMax] = {
		-455, -686, -414, -588};
const static s16 CouplingsPosAdj[DirMax] = {
		-421, 0, -108, -175};

//	Channel 0~3 	Steering Motor Position Encoder
s32 GetSterringMotorPosition(Dir_TypeDef Direction)
{
	s32 Value;

	Value = GetSpiBuffer((u8)Direction)
			+ SterringMotorPosAdj[Direction]
			+ GetCouplingsPosition(Direction);
	return Value;
}

//	Channel 4~7 	Couplings Position Encoder
s32 GetCouplingsPosition(Dir_TypeDef Direction)
{
	s32 Value;

	Value = GetSpiBuffer((u8)(Direction+4))
			+ CouplingsPosAdj[Direction];
	if(Value>512){
		Value -= 1024;
	}else if(Value<-512){
		Value += 1024;
	}
	return Value;
}