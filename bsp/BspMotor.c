/*
* 本代码主要负责电机发送指令的书写
* 未检测是否发送成功
*/
#include "BspMotor.h"
#include "can.h"

static CAN_TxHeaderTypeDef  can_tx_message_l;
static CAN_TxHeaderTypeDef  can_tx_message_2;
static CAN_TxHeaderTypeDef  can_tx_message_3;
static CAN_TxHeaderTypeDef  can_tx_message_4;

static uint8_t              can_send_data_l[8];

void FireCMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
    uint32_t send_mail_box_l;
    can_tx_message_l.StdId = FireAllId;
    can_tx_message_l.IDE = CAN_ID_STD;
    can_tx_message_l.RTR = CAN_RTR_DATA;
    can_tx_message_l.DLC = 0x08;
    can_send_data_l[0] = motor1 >> 8;
    can_send_data_l[1] = motor1;
    can_send_data_l[2] = motor2 >> 8;
    can_send_data_l[3] = motor2;
    can_send_data_l[4] = motor3 >> 8;
    can_send_data_l[5] = motor3;
    can_send_data_l[6] = motor4 >> 8;
    can_send_data_l[7] = motor4;

    HAL_CAN_AddTxMessage(&hcan1, &can_tx_message_l, can_send_data_l, &send_mail_box_l);
}

void Position_CMD(int16_t Pitch, int16_t Yaw,int16_t Ammo_Speed)
{
    uint32_t send_mail_box_l;
    can_tx_message_2.StdId = Pitch_Ammo_MotorTxId;
    can_tx_message_2.IDE = CAN_ID_STD;
    can_tx_message_2.RTR = CAN_RTR_DATA;
    can_tx_message_2.DLC = 0x04;
    can_send_data_l[0] = Pitch >> 8;
    can_send_data_l[1] = Pitch;
		can_send_data_l[2] = Ammo_Speed >> 8;
    can_send_data_l[3] = Ammo_Speed;

    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message_2, can_send_data_l, &send_mail_box_l);
	
		uint32_t send_mail_box_2;
    can_tx_message_3.StdId = YawMotorTxId;
    can_tx_message_3.IDE = CAN_ID_STD;
    can_tx_message_3.RTR = CAN_RTR_DATA;
    can_tx_message_3.DLC = 0x02;
    can_send_data_l[0] = Yaw >> 8;
    can_send_data_l[1] = Yaw;

    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message_3, can_send_data_l, &send_mail_box_2);
}
