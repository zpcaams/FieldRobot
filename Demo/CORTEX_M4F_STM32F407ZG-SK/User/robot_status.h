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
#include "robot_putter.h"
#include "robot_driver.h"
#include "robot_move.h"
#include "robot_test.h"
#include "robot_adc.h"
 
/************************** Constant Definitions *****************************/

/* 
 * ������״̬��˵��
 * 
 * ��ʼ������ʼ������ģ�����
 * ���У����������Ĭ��λ�ã��ٶ�Ϊ��
 * ƽ������������ͬ�Ƕȣ���ͬ�ٶ��ƶ�
 * ת�䣺���ո���ת��뾶����
 * ��ת��ԭ����ת��ת
 * �߶ȵ��ڣ��Ƹ˵��ڳ���
 * �־���ڣ����Ȼ�Բ��--ֻ��һ�ȶ�
 * 
 * ��ʼ����״̬��
 * �ϵ磺������Ĭ��״̬
 * �Լ죺������ģ���Ƿ�������
 * �������Լ���ϣ����Խ���ָ�״̬תΪ����
 * 
 * ������״̬��
 * ң�����ܿ��ش򿪣�����ģʽѡ��
 * ĳһ����״̬�£�
 * ң�����ܿ��عرգ�ֹͣ������������
 * ֹͣ״̬��
 * �ȴ����ֹͣ��ת�󣬻ص�����״̬
 * 
 * ͨ��	ң����λ��	��������	ƽ��	ת��	��ת	�߶�	�־�
 * ͨ��1	����ҡ������	ת��		O	O	X	X	X
 * ͨ��2	����ҡ������	����		X	X	X	O	O
 * ͨ��3	����ҡ������	�ٶ�		O	O	O	X	X
 * ͨ��4	����ҡ������	ȷ��/ȡ��X	X	X	X	O
 * ͨ��5	��ǰ��ť		�߶�		X	X	X	O	X
 * ͨ��6	�� ��ť		�־�		X	X	X	O	O
 * ͨ��7	��ǰ��ť		ģʽѡ��	X	X	X	X	X
 * ͨ��8	��ǰ�Ͽ���	�ܿ���
 * 
 * �߶ȵ��ڣ�
 * ��ǰ��ť��������λ�ã�����ҡ�����¿������ţ�����ťѡ����
 * 
 * �־���ڣ�
 * ��ǰ��ť��������λ�ã�����ťѡ���ȣ�����ҡ�����ҿ�����̥ת�빤��λ�ã�����ҡ�����¿�����̥�ٶ�
 * 
 * ע�⣡
 * ����ҡ�����£����ţ�ͨ��������Ӱ��
 * ����ť������΢����ͨ������
 * ��������ͨ������ͬʱ����
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