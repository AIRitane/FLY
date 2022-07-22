#include "DV_task.h"
#include "system.h"

void DV_task(void const * argument)
{
  for(;;)
  {
		printf("%d,%d,%d,%d",shoot.Fire_Motor[0]->speed_rpm,shoot.Fire_Motor[1]->speed_rpm,\
					shoot.Fire_Motor[2]->speed_rpm,shoot.Fire_Motor[3]->speed_rpm);
    osDelay(1);
  }
}


