#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"

#define YAW_ID 4
#define PITCH_ID 5
#define SHOOT_0 0
#define SHOOT_1 1
#define TRIGLE 2

/* CAN send and receive ID */
typedef enum
{
	
    CAN_ALL_ID_L = 0x200,
    CAN_Motor1_ID = 0x201,
    CAN_Motor2_ID = 0x202,
    CAN_Motor3_ID = 0x203,
    CAN_Motor4_ID = 0x204,

    CAN_ALL_ID_H = 0x1FF,
    CAN_Motor5_ID = 0x205,
    CAN_Motor6_ID = 0x206,
    CAN_Motor7_ID = 0x207,
	CAN_Motor8_ID = 0x208,
    

} can_msg_id_e;

//rm motor data
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;
    int16_t given_current;
    uint8_t temperate;
    uint16_t last_ecd;
    
    int32_t series_ecd;
    fp32 angle;
    fp32 series_angle;
		fp32 speed;
    int32_t rounds;
} motor_measure_t;

extern void can_filter_init(void);

extern void CAN1_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

extern void CAN2_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);

extern const motor_measure_t *get_can1_motor_measure_point(uint8_t i);

extern const motor_measure_t *get_can2_motor_measure_point(uint8_t i);

const motor_measure_t *get_yaw_gimbal_motor_measure_point(void);
const motor_measure_t *get_pitch_gimbal_motor_measure_point(void);
const motor_measure_t *get_shoot0_motor_measure_point(void);
const motor_measure_t *get_shoot1_motor_measure_point(void);
const motor_measure_t *get_trigle_motor_measure_point(void);
#endif
