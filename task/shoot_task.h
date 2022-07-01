#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H

#include "system.h"

typedef struct
{
	//反馈量
	const RC_ctrl_t *RC;
	const motor_measure_t *Fire_Motor[4];
	const motor_measure_t *Yaw_Motor;
	const motor_measure_t *Pitch_Motor;
	const motor_measure_t *Ammo_Motor;

	//设定量
	float pitch;
	float yaw;
	float ammo_speed;
	float fire_speed;
	
	//PID
	pid_type_def FirePid[4];
	pid_type_def PitchPid;
	pid_type_def YawPid;
	pid_type_def AmmoPid;
	
	//
	int16_t pitch_current;
	int16_t yaw_current;
	int16_t ammo_speed_current;
	int16_t fire_speed_current[4];
}shoot_t;


#endif
