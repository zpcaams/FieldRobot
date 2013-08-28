/*****************************************************************************/
/**
*
* @file robot_status.h
*
*
******************************************************************************/

#ifndef __ROBOT_STATUS_H
#define __ROBOT_STATUS_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

/* Robot includes */
#include "robot_common.h"
#include "robot_remote_control.h"
#include "robot_driver.h"
#include "robot_move.h"
#include "robot_test.h"
#include "robot_spi.h"
#include "robot_adc.h"
 
/************************** Constant Definitions *****************************/

/* 
 * 机器人状态及说明
 * 
 * 初始化：初始化各个模块参数
 * 空闲：各个电机在默认位置，速度为零
 * 平动：各轮以相同角度，相同速度移动
 * 转弯：按照给定转弯半径行走
 * 回转：原地正转或反转
 * 高度调节：推杆调节长度
 * 轮距调节：单腿划圆弧--只可一腿动
 * 
 * 初始化下状态：
 * 上电：启动后默认状态
 * 自检：检查各个模块是否工作正常
 * 工作：自检完毕，可以接收指令，状态转为空闲
 * 
 * 空闲下状态：
 * 遥控器总开关打开，根据模式选择
 * 某一工作状态下：
 * 遥控器总开关关闭，停止驱动器工作，
 * 停止状态：
 * 等待电机停止运转后，回到空闲状态
 * 
 * 通道	遥控器位置	基本功能	平动	转弯	回转	高度	轮距
 * 通道1	右手摇杆左右	转角		O	O	X	X	X
 * 通道2	左手摇杆下上	油门		X	X	X	O	O
 * 通道3	右手摇杆下上	速度		O	O	O	X	X
 * 通道4	左手摇杆左右	确认/取消X	X	X	X	O
 * 通道5	左前旋钮		高度		X	X	X	O	X
 * 通道6	左 旋钮		轮距		X	X	X	O	O
 * 通道7	右前旋钮		模式选择	X	X	X	X	X
 * 通道8	右前上开关	总开关
 * 
 * 高度调节：
 * 左前旋钮控制最终位置，左手摇杆上下控制油门，左旋钮选择腿
 * 
 * 轮距调节：
 * 左前旋钮控制最终位置，左旋钮选择腿，左手摇杆左右控制轮胎转入工作位置，左手摇杆上下控制轮胎速度
 * 
 * 注意！
 * 右手摇杆上下（油门，通道三）会影响
 * 左旋钮（油门微调，通道六）
 * 故这两个通道不能同时动作
 */
#define	ROBOT_INIT			0
#define	ROBOT_IDLE			1
#define	ROBOT_MOVE			2
#define	ROBOT_MOVE_STOP		3
#define	ROBOT_TURN			4
#define	ROBOT_TURN_STOP		5
#define	ROBOT_ROLL			6
#define	ROBOT_ROLL_STOP		7
#define	ROBOT_HEIGHT		8
#define	ROBOT_HEIGHT_STOP	9
#define	ROBOT_WIDTH			10
#define	ROBOT_WIDTH_STOP	11
#define	ROBOT_TEST			12
#define	ROBOT_TEST_STOP		13
 
#define	RobotMain_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotMove_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotMoveStop_TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )
#define	RobotTest_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define	RobotTestStop_TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )
 
 
 
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void GreatRobotMainTask(void); 
void ResetRoborBusy(void);

#ifdef __cplusplus
}
#endif

#endif /* __ROBOT_STATUS_H */