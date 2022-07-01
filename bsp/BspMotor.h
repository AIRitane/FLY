#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"

/* CAN send and receive ID */
typedef enum
{
    FireAllId = 0x200,
    CanMotor1Id = 0x201,
    CanMotor2Id = 0x202,
    CanMotor3Id = 0x203,
    CanMotor4Id = 0x204
} ChassisID;

typedef enum
{
	Pitch_Ammo_MotorTxId = 0x200,
	PitchMotorRecId = 0x201,
	YawMotorRecId = 0x205,
	AmmoMotorRecId = 0x202,
  YawMotorTxId = 0x1ff

} YawID;
//rm motor data
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    uint16_t last_ecd;
	
    fp32 angle;
	fp32 speed;
} motor_measure_t;

void FireCMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
void Position_CMD(int16_t Pitch, int16_t Yaw,int16_t Ammo_Speed);

#endif
