#include "system.h"

void sys_int()
{
	//必要延迟，主要是编码器初始化造成的，否则CAN2总线瘫痪
	HAL_Delay(1000);
	//LED初始化
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	//电机初始化
	can_filter_init();
	//遥控器初始化
	remote_control_init();
}