/**
	******************************************************************************
	* @file           : bsp_motor.c
	* @brief          : Main program body
	******************************************************************************
	* @config	:	prescaler = 3; BS1 = 10TD; BS2 = 3TD; SYNC_JumpWidth = 1TD;
	*				Baud Rate = 1,000,000 Bit/s
	* @io		:	CAN1_TX = PD0, CAN1_RX = PD1, CAN2_TX = PB12, CAN2_RX = PB13
	* @interrups:	CAN1_RX0(FIFO0)、CAN2_RX0(FIFO0)
	* @functions:	void CAN1_CMD(...);
	* 				void CAN2_CMD(...);
	*				const motor_measure_t *get_can1_motor_measure_point(uint8_t i);
	*				const motor_measure_t *get_can2_motor_measure_point(uint8_t i);
	******************************************************************************
	*/


#include "bsp_motor.h"
#include "can.h"

//can数据解包
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
        (ptr)->angle = (ptr)->ecd / 8192.0f * 360.0f - 180.0f;          \
				(ptr)->speed = (ptr)->speed_rpm * 360.0f;						\
    }

void process_motor_measure(motor_measure_t *motor_measure);    
    


motor_measure_t can1_motor[8];
motor_measure_t can2_motor[8];

static CAN_TxHeaderTypeDef  can_tx_message_h;
static uint8_t              can_send_data_h[8];
static CAN_TxHeaderTypeDef  can_tx_message_l;
static uint8_t              can_send_data_l[8];

    

void can_filter_init(void);
void CAN1_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);
void CAN2_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8);
const motor_measure_t *get_can1_motor_measure_point(uint8_t i);
const motor_measure_t *get_can2_motor_measure_point(uint8_t i);


/**
  * @brief          电机过零点计数
  * @param[in]      电机传感器值结构体变量
  * @retval         none
  */    
void process_motor_measure(motor_measure_t *motor_measure)
{
    if ((motor_measure->ecd - motor_measure->last_ecd) > 6400)
    {
        motor_measure->rounds++;
    }
    else if ((motor_measure->ecd - motor_measure->last_ecd) < -6400)
    {
        motor_measure->rounds--;
    }
    
    motor_measure->series_ecd = motor_measure->ecd + 8192* motor_measure->rounds;
    motor_measure->series_angle = motor_measure->angle  + 360* motor_measure->rounds;
}

    
    
/**
  * @brief          can1和can2的滤波器初始化
  * @param[in]      none
  * @retval         none
  */    
void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);


    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    can_filter_st.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

}





/**
  * @brief          can1发送电机控制电流(0x200下的0x201,0x202,0x203,0x204; 0x1FF下的0x205,0x206,0x207,0x208)
  * @param[in]      can1下8个电机的供给电流控制，范围：3508:[-16384,16384]; 6020:[-30000,30000]; 2006:[-10000,10000];
  * @retval         none
  */


void CAN1_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    uint32_t send_mail_box_l;
    can_tx_message_l.StdId = CAN_ALL_ID_L;
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
    
    uint32_t send_mail_box_h;
    can_tx_message_h.StdId = CAN_ALL_ID_H;
    can_tx_message_h.IDE = CAN_ID_STD;
    can_tx_message_h.RTR = CAN_RTR_DATA;
    can_tx_message_h.DLC = 0x08;
    can_send_data_h[0] = (motor5 >> 8);
    can_send_data_h[1] = motor5;
    can_send_data_h[2] = (motor6 >> 8);
    can_send_data_h[3] = motor6;
    can_send_data_h[4] = (motor7 >> 8);
    can_send_data_h[5] = motor7;
    can_send_data_h[6] = (motor8 >> 8);
    can_send_data_h[7] = motor8;
    
    HAL_CAN_AddTxMessage(&hcan1, &can_tx_message_h, can_send_data_h, &send_mail_box_h);
}

/**
  * @brief          can2发送电机控制电流(0x200下的0x201,0x202,0x203,0x204; 0x1FF下的0x205,0x206,0x207,0x208)
  * @param[in]      can2下8个电机的供给电流控制，范围：3508:[-16384,16384]; 6020:[-30000,30000]; 2006:[-10000,10000];
  * @retval         none
  */
void CAN2_CMD(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4, int16_t motor5, int16_t motor6, int16_t motor7, int16_t motor8)
{
    uint32_t send_mail_box_l;
    can_tx_message_l.StdId = CAN_ALL_ID_L;
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

    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message_l, can_send_data_l, &send_mail_box_l);
    
    uint32_t send_mail_box_h;
    can_tx_message_h.StdId = CAN_ALL_ID_H;
    can_tx_message_h.IDE = CAN_ID_STD;
    can_tx_message_h.RTR = CAN_RTR_DATA;
    can_tx_message_h.DLC = 0x08;
    can_send_data_h[0] = (motor5 >> 8);
    can_send_data_h[1] = motor5;
    can_send_data_h[2] = (motor6 >> 8);
    can_send_data_h[3] = motor6;
    can_send_data_h[4] = (motor7 >> 8);
    can_send_data_h[5] = motor7;
    can_send_data_h[6] = (motor8 >> 8);
    can_send_data_h[7] = motor8;
    
    HAL_CAN_AddTxMessage(&hcan2, &can_tx_message_h, can_send_data_h, &send_mail_box_h);
}


/**
  * @brief          hal库CAN1 fifo0回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

	
	
    switch (rx_header.StdId)
    {
        case CAN_Motor1_ID:
        case CAN_Motor2_ID:
        case CAN_Motor3_ID:
        case CAN_Motor4_ID:
        case CAN_Motor5_ID:
        case CAN_Motor6_ID:
        case CAN_Motor7_ID:
        case CAN_Motor8_ID:
        {
            static uint8_t i = 0;
            i = rx_header.StdId - CAN_Motor1_ID;
			if (hcan->Instance == CAN1)
			{
				get_motor_measure(&can1_motor[i], rx_data);
				process_motor_measure(&can1_motor[i]);
			}
			else if (hcan->Instance == CAN2)
			{
				get_motor_measure(&can2_motor[i], rx_data);
				process_motor_measure(&can2_motor[i]);
			}
            
            
            break;
        }

        default:
        {
            break;
        }
    }
}
    
    


/**
  * @brief          返回can1电机数据指针
  * @param[in]      i: 电机编号,范围[0,7]
  * @retval         电机数据指针
  */
const motor_measure_t *get_can1_motor_measure_point(uint8_t i)
{
    return &can1_motor[(i & 0x07)];
}

/**
  * @brief          返回can2电机数据指针
  * @param[in]      i: 电机编号,范围[0,7]
  * @retval         电机数据指针
  */
const motor_measure_t *get_can2_motor_measure_point(uint8_t i)
{
    return &can2_motor[(i & 0x07)];
}


const motor_measure_t *get_yaw_gimbal_motor_measure_point(void)
{
	return get_can2_motor_measure_point(YAW_ID);
}
const motor_measure_t *get_pitch_gimbal_motor_measure_point(void)
{
	return get_can2_motor_measure_point(PITCH_ID);
}
const motor_measure_t *get_shoot0_motor_measure_point(void)
{
	return get_can2_motor_measure_point(SHOOT_0);
}
const motor_measure_t *get_shoot1_motor_measure_point(void)
{
	return get_can2_motor_measure_point(SHOOT_1);
}
const motor_measure_t *get_trigle_motor_measure_point(void)
{
	return get_can2_motor_measure_point(TRIGLE);
}
