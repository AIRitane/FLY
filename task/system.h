#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stdio.h"

#include "tim.h"
#include "cmsis_os.h"

#include "pid.h"
#include "user_lib.h"

#include "CanReceiveDecom.h"
#include "BspMotor.h"
#include "remote_control.h"
#include "struct_typedef.h"
#include "encode.h"
#include "RefereeBehaviour.h"
#include "shoot_task.h"
#include "RM_Cilent_UI.h"

void sys_int();

#endif
