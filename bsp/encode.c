#include "encode.h"
#include "system.h"


encoder_t encoder_yaw;
encoder_t encoder_pitch;

const encoder_t *get_yaw_encoder_point()
{
	return &encoder_yaw;
}

const encoder_t *get_pitch_encoder_point()
{
	return &encoder_pitch;
}
