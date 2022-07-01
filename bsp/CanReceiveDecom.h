#ifndef __CANRECEIVEDECOME_H
#define __CANRECEIVEDECOME_H

#include "struct_typedef.h"
#include "BspMotor.h"

void can_filter_init(void);
const motor_measure_t *GetFireMeasure(uint8_t i);
const motor_measure_t *GetYawMeasure(void);
const motor_measure_t *GetPitchMeasure(void);
const motor_measure_t *GetAmmoMeasure(void);

#endif
