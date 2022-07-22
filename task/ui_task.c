#include "system.h"

UI_Data_t UI_Data;

void ui_task(void const * argument)
{
	UI_Data.yaw = shoot.encode_yaw->angle;
	UI_Data.pitch = shoot.encode_pitch->angle;
	UI_Data.pitch_turns = shoot.encode_pitch->turns;
	UI_Data.time = dart_remaining_time_t.dart_remaining_time;
  for(;;)
  {
		workshop_communication(UI_Data);
    osDelay(10);
  }
}