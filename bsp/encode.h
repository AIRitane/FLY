#ifndef __ENCODE_H
#define __ENCODE_H

#include "struct_typedef.h"

typedef struct
{
	float angle;
	int16_t turns;
	float palstance;
}encoder_t;


#define get_encoder_data(ptr, data)																							\
			{														 																							\
				(ptr)->angle = ((int16_t)((data)[3] << 8 | (data)[2]))* 360 / 32768.0f;			\
				(ptr)->turns = (int16_t)((data)[7] << 8 | (data)[6]);										\
				(ptr)->palstance = ((int16_t)((data)[5] << 8 | (data)[4]))* 360 / 32768.0f * 10; \
			}																																					\

extern encoder_t encoder_yaw;	
extern encoder_t encoder_pitch;
			
const encoder_t *get_yaw_encoder_point();
const encoder_t *get_pitch_encoder_point();
			
#endif
