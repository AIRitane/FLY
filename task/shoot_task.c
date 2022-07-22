#include "shoot_task.h"
#include "system.h"

shoot_t shoot;
//调整参数
float Fire_FSpeed = 0.412;
float Fire_BSpeed = 0.112;
float Fire_Pid[4][3]={{20000,0,0},
											{20000,0,0},
											{20000,0,0},
											{20000,0,0}};
//灵敏度
float pitch_speed = 0.0005;
float yaw_speed = 0.01;
											
//不用调整
float YawPid_Speed[3] = {20000,30.0000,0};
float YawPid_Gory[3] = {0.50f,0.0000,0};
float Pitch_Speed[3] = {80000,0000,0};
float Pitch_Gory[3] = {1.0f,0.00002,0};
float AmmoPid[3] = {80000,0.0000,0};

float Ammo_Speed = 0.7;

uint8_t down_flag = 0;
uint8_t up_flag = 0;

void shoot_init();
void shoot_setloop();
void shoot_current();

void shoot_task(void const *argument)
{
	shoot_init();
	
  while (1)
  {
		shoot_setloop();
		shoot_current();
		FireCMD(shoot.fire_speed_current[0], shoot.fire_speed_current[1], shoot.fire_speed_current[2], shoot.fire_speed_current[3]);
		Position_CMD(shoot.pitch_current, shoot.yaw_current,shoot.ammo_speed_current);
		
    osDelay(1);
  }
}

void shoot_init()
{
	//调用数据接口
	shoot.RC = get_remote_control_point();
	for(uint8_t i=0;i<4;i++)
		shoot.Fire_Motor[i] = GetFireMeasure(i);
	shoot.Ammo_Motor = GetAmmoMeasure();
	shoot.Pitch_Motor = GetPitchMeasure();
	shoot.Yaw_Motor = GetYawMeasure();
	shoot.encode_yaw = get_yaw_encoder_point();
	shoot.encode_pitch = get_pitch_encoder_point();
	
	//pid初始化
	for(uint8_t i=0;i<4;i++)
		PID_init(&shoot.FirePid[i],PID_POSITION,Fire_Pid[i],16300,5000);
	PID_init(&shoot.YawPid_Speed,PID_POSITION,YawPid_Speed,30000,5000);
	PID_init(&shoot.YawPid_Gory,PID_ANGLE,YawPid_Gory,10,5000);
	PID_init(&shoot.Pitch_Speed,PID_POSITION,Pitch_Speed,30000,5000);
	PID_init(&shoot.Pitch_Gory,PID_POSITION,Pitch_Gory,10,5);
	PID_init(&shoot.AmmoPid,PID_POSITION,AmmoPid,10000,5000);

	//初始化数值
	shoot.yaw = INIT_ANGLE;
	shoot.pitch_turns = PITCH_TURNS;
	shoot.pitch = INIT_PITCH;
}

void shoot_setloop()
{
	if(switch_is_down(shoot.RC->rc.s[1]))//裁判系统模式
	{
		//
		shoot.yaw += student_interactive_data_t.Yaw/660.0f*yaw_speed;
		//
		if(shoot.yaw > YAW_MAX) shoot.yaw = YAW_MAX;
		if(shoot.yaw < YAW_MIN) shoot.yaw = YAW_MIN;
		
		//
		shoot.pitch += student_interactive_data_t.Pitch/660.0f*pitch_speed;
		if(shoot.pitch > 180)
		{
			shoot.pitch = theta_format(shoot.pitch);
			shoot.pitch_turns += 1;
		}
		else if(shoot.pitch < -180)
		{
			shoot.pitch = theta_format(shoot.pitch);
			shoot.pitch_turns -= 1;
		}
		//
		if((shoot.pitch_turns*360 + shoot.pitch) > (PITCH_TURNS_MAX*360 + PITCH_MAX))
		{
			shoot.pitch_turns = PITCH_TURNS_MAX;
			shoot.pitch = PITCH_MAX;
		}
		if((shoot.pitch_turns*360 + shoot.pitch) < (PITCH_TURNS_MIN*360 + PITCH_MIN))
		{
			shoot.pitch_turns = PITCH_TURNS_MIN;
			shoot.pitch = PITCH_MIN;
		}
		
		//
		if(dart_client_cmd_t.dart_launch_opening_status ==  0)//已经开启
		{
			shoot.fire_fspeed = Fire_FSpeed;
			shoot.fire_bspeed = Fire_BSpeed;
			shoot.ammo_speed = -Ammo_Speed;
			
			if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_6)==0)
			{
				shoot.ammo_speed = -shoot.RC->rc.ch[4]/660.0f*Ammo_Speed;
				if(shoot.ammo_speed>0) shoot.ammo_speed = 0;
			}
			if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7)==0)
			{
				shoot.ammo_speed = -shoot.RC->rc.ch[4]/660.0f*Ammo_Speed;
				if(shoot.ammo_speed<0) shoot.ammo_speed = 0;
			}
		}
		else if(dart_client_cmd_t.dart_launch_opening_status ==  2)//未完全开启,预热
		{
			shoot.fire_fspeed = Fire_FSpeed;
			shoot.fire_bspeed = Fire_BSpeed;
			shoot.ammo_speed = 0;
		}
		else
		{
			shoot.fire_fspeed = 0;
			shoot.fire_bspeed = 0;
			shoot.ammo_speed = 0;
		}
	}
	//调整位姿时，失能发射
	else if(switch_is_up(shoot.RC->rc.s[1]))
	{
		shoot.yaw += shoot.RC->rc.ch[2]/660.0f*yaw_speed;
		//YAW限幅
		if(shoot.yaw > YAW_MAX) shoot.yaw = YAW_MAX;
		if(shoot.yaw < YAW_MIN) shoot.yaw = YAW_MIN;
		
		shoot.pitch += shoot.RC->rc.ch[1]/660.0f*pitch_speed;
		if(shoot.pitch > 180)
		{
			shoot.pitch = theta_format(shoot.pitch);
			shoot.pitch_turns += 1;
		}
		else if(shoot.pitch < -180)
		{
			shoot.pitch = theta_format(shoot.pitch);
			shoot.pitch_turns -= 1;
		}
		//pitch限幅
		if((shoot.pitch_turns*360 + shoot.pitch) > (PITCH_TURNS_MAX*360 + PITCH_MAX))
		{
			shoot.pitch_turns = PITCH_TURNS_MAX;
			shoot.pitch = PITCH_MAX;
		}
		if((shoot.pitch_turns*360 + shoot.pitch) < (PITCH_TURNS_MIN*360 + PITCH_MIN))
		{
			shoot.pitch_turns = PITCH_TURNS_MIN;
			shoot.pitch = PITCH_MIN;
		}
		
		shoot.fire_fspeed = 0;
		shoot.fire_bspeed = 0;
		shoot.ammo_speed = 0;
	}
	else
	{
		//准备发射
		if(switch_is_mid(shoot.RC->rc.s[0]))
		{
			shoot.fire_fspeed = Fire_FSpeed;
			shoot.fire_bspeed = Fire_BSpeed;
			shoot.ammo_speed = -shoot.RC->rc.ch[4]/660.0f*Ammo_Speed;
			if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_6)==0)
			{
				shoot.ammo_speed = -shoot.RC->rc.ch[4]/660.0f*Ammo_Speed;
				if(shoot.ammo_speed>0) shoot.ammo_speed = 0;
			}
			if(HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_7)==0)
			{
				shoot.ammo_speed = -shoot.RC->rc.ch[4]/660.0f*Ammo_Speed;
				if(shoot.ammo_speed<0) shoot.ammo_speed = 0;
			}
		}
		else
		{
			shoot.fire_fspeed = 0;
			shoot.fire_bspeed = 0;
			shoot.ammo_speed = 0;
		}
	}
}

float pitch_error_angle = 0;
void shoot_current()
{
	float yaw_goryset = 0;
	float pitch_goryset = 0;
	
	
	shoot.ammo_speed_current = PID_calc(&shoot.AmmoPid, shoot.Ammo_Motor->speed_rpm * 0.00011375f, shoot.ammo_speed);
	
	yaw_goryset = -PID_calc(&shoot.YawPid_Gory, shoot.encode_yaw->angle, shoot.yaw)/10.0f;
	shoot.yaw_current = PID_calc(&shoot.YawPid_Speed, shoot.Yaw_Motor->speed_rpm * 0.002976f, yaw_goryset);
	
	pitch_error_angle = (shoot.pitch_turns * 360 + shoot.pitch) - (shoot.encode_pitch->turns*360 + shoot.encode_pitch->angle);
	pitch_goryset = PID_calc(&shoot.Pitch_Gory, pitch_error_angle, 0)/10.0f;
	shoot.pitch_current = PID_calc(&shoot.Pitch_Speed, shoot.Pitch_Motor->speed_rpm * 0.00011375f, pitch_goryset);
	
	shoot.fire_speed_current[0] = PID_calc(&shoot.FirePid[0], shoot.Fire_Motor[0]->speed_rpm * 0.00011375f, -shoot.fire_bspeed);
	shoot.fire_speed_current[1] = PID_calc(&shoot.FirePid[1], shoot.Fire_Motor[1]->speed_rpm * 0.00011375f, shoot.fire_fspeed);
	shoot.fire_speed_current[2] = PID_calc(&shoot.FirePid[2], shoot.Fire_Motor[2]->speed_rpm * 0.00011375f, shoot.fire_bspeed);
	shoot.fire_speed_current[3] = PID_calc(&shoot.FirePid[3], shoot.Fire_Motor[3]->speed_rpm * 0.00011375f, -shoot.fire_fspeed);
}
