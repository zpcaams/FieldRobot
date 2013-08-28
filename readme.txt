中国农业机械化科学研究院
机电技术应用与研究所
田间机器人主控程序
硬件基于stm32f407-discovery板
软件基于FreeRTOS CORTEX_M4F_STM32F407ZG-SK Demo
作者：周鹏
日期：2013-8-24

主文件夹下目录由FreeRTOS V7.4.0 Demo继承而来，
删除了其他Demo Port，并在Demo/CORTEX_M4F_STM32F407ZG-SK/Libraries文件夹下添加了以下的库：
	CMSIS
	STM32_USB_Device_Library
	STM32_USB_HOST_Library
	STM32_USB_OTG_Driver
	STM32F4xx_StdPeriph_Driver

Demo/CORTEX_M4F_STM32F407ZG-SK\User 是主要工作文件夹，机器人控制代码将在这里实现。