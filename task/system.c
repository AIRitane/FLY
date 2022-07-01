#include "system.h"

void sys_int()
{
	//LED初始化
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	//电机初始化
	can_filter_init();
	//遥控器初始化
	remote_control_init();
}