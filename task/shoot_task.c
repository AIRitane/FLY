#include "shoot_task.h"
#include "cmsis_os.h"


shoot_t shoot;
float Fire_Pid[4][3]={{80000,0,0},
											{80000,0,0},
											{80000,0,0},
											{80000,0,0}};
float YawPid[3] = {80000,0.0000,0};
float PitchPid[3] = {80000,0.0000,0};
float AmmoPid[3] = {0.005,0.0000,0};

float Ammo_Speed = 0.6;
float Fire_Speed = 0.3;
float pitch_speed = 0.2;
float yaw_speed = 0.1;

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
	shoot.RC = get_remote_control_point();
	for(uint8_t i=0;i<4;i++)
		shoot.Fire_Motor[i] = GetFireMeasure(i);
	shoot.Ammo_Motor = GetAmmoMeasure();
	shoot.Pitch_Motor = GetPitchMeasure();
	shoot.Yaw_Motor = GetYawMeasure();
	for(uint8_t i=0;i<4;i++)
		PID_init(&shoot.FirePid[i],PID_POSITION,Fire_Pid[i],16300,5000);
	PID_init(&shoot.YawPid,PID_POSITION,YawPid,30000,5000);
	PID_init(&shoot.PitchPid,PID_POSITION,YawPid,16300,5000);
	PID_init(&shoot.AmmoPid,PID_POSITION,YawPid,10000,5000);
}

void shoot_setloop()
{
	//调整位姿时，失能发射
	if(switch_is_up(shoot.RC->rc.s[1]))
	{
		shoot.yaw = shoot.RC->rc.ch[2]/660.0f*yaw_speed;
		shoot.pitch = shoot.RC->rc.ch[1]/660.0f*pitch_speed;
		shoot.fire_speed = 0;
		shoot.ammo_speed = 0;
	}
	else
	{
		shoot.yaw = 0;
		shoot.pitch = 0;
		
		//准备发射
		if(switch_is_mid(shoot.RC->rc.s[0]))
		{
			shoot.fire_speed = Fire_Speed;
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
			shoot.fire_speed = 0;
			shoot.ammo_speed = 0;
		}
	}
}

void shoot_current()
{
	shoot.ammo_speed_current = PID_calc(&shoot.AmmoPid, shoot.Ammo_Motor->speed_rpm * 0.00011375f, shoot.ammo_speed);
	shoot.yaw_current = PID_calc(&shoot.YawPid, shoot.Yaw_Motor->speed_rpm * 0.00011375f, shoot.yaw);
	shoot.pitch_current = PID_calc(&shoot.PitchPid, shoot.Pitch_Motor->speed_rpm * 0.00011375f, shoot.pitch);
	
	shoot.fire_speed_current[0] = PID_calc(&shoot.FirePid[0], shoot.Fire_Motor[0]->speed_rpm * 0.00011375f, -shoot.fire_speed);
	shoot.fire_speed_current[1] = PID_calc(&shoot.FirePid[1], shoot.Fire_Motor[1]->speed_rpm * 0.00011375f, shoot.fire_speed);
	shoot.fire_speed_current[2] = PID_calc(&shoot.FirePid[2], shoot.Fire_Motor[2]->speed_rpm * 0.00011375f, shoot.fire_speed);
	shoot.fire_speed_current[3] = PID_calc(&shoot.FirePid[3], shoot.Fire_Motor[3]->speed_rpm * 0.00011375f, -shoot.fire_speed);
}
