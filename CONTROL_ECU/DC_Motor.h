/*
 *  Module: DC_Motor
 *
 *  Created on: Oct 5, 2024
 *
 *  Author: Mahmoud Hashish
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

typedef enum {
	CW, ACW, STOP
}DcMotor_State;

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_State state, uint8 fan_speed);

#endif /* DC_MOTOR_H_ */
