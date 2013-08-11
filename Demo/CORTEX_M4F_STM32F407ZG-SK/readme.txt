
中国农业机械化科学研究院
机电技术应用与研究所
田间机器人主控程序
硬件基于stm32f407-discovery板
软件基于FreeRTOS CORTEX_M4F_STM32F407ZG-SK Demo

User/robot_status.c     机器人状态
User/robot_driver.c     电机驱动器（CAN）
User/robot_encoder.c    绝对值编码器及2.4G遥控器（SPI）
User/robot_debug.c      调试输出（Usart）
TODO
User/robot_putter.c     推杆状态（ADC）
User/robot_pc.c         上位机（CAN）