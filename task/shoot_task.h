#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H

#include "struct_typedef.h"
#include "CanReceiveDecom.h"
#include "remote_control.h"
#include "encode.h"
#include "pid.h"

#define INIT_ANGLE 110
#define PITCH_TURNS 0;
#define INIT_PITCH 178;
#define PITCH_TURNS_MAX 1
#define PITCH_TURNS_MIN 0
#define PITCH_MAX -162
#define PITCH_MIN 153
#define YAW_MAX 119
#define YAW_MIN 105
typedef struct
{
	//反馈量
	const RC_ctrl_t *RC;
	const motor_measure_t *Fire_Motor[4];
	const motor_measure_t *Yaw_Motor;
	const motor_measure_t *Pitch_Motor;
	const motor_measure_t *Ammo_Motor;
	const encoder_t *encode_yaw;
	const encoder_t *encode_pitch;
	
	

	//设定量
	float pitch;
	float yaw;
	float pitch_turns;
	float ammo_speed;
	float fire_fspeed;
	float fire_bspeed;
	
	//PID
	pid_type_def FirePid[4];
	pid_type_def Pitch_Speed;
	pid_type_def Pitch_Gory;
	pid_type_def YawPid_Speed;
	pid_type_def YawPid_Gory;
	pid_type_def AmmoPid;
	
	//电流值
	int16_t pitch_current;
	int16_t yaw_current;
	int16_t ammo_speed_current;
	int16_t fire_speed_current[4];
}shoot_t;

extern shoot_t shoot;

#endif
