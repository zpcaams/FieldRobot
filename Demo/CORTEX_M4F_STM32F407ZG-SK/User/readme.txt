中国农业机械化科学研究院
机电技术应用与研究所
田间机器人主控程序
作者：周鹏
日期：2013-8-28

软件层次结构
顶层--
	robot_status.c	机器人状态

	robot_init.c	初始化
	robot_move.c	机器人平移状态
	robot_test.c	机器人测试

设备驱动层--
	robot_couplings.c     	联轴器控制
	robot_debug.c		调试输出
	robot_driver.c     	电机驱动器控制
	robot_putter.c		推杆控制
	robot_position.c	电机位置信息
	robot_remote_control.c	遥控器信息
底层--
	robot_adc.c
	robot_can.c
	robot_spi.c