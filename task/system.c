#include "system.h"

void sys_int()
{
	//LED��ʼ��
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	//�����ʼ��
	can_filter_init();
	//ң������ʼ��
	remote_control_init();
}