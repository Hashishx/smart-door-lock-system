/*
 *  Module: DC_Motor
 *
 *  Created on: Oct 5, 2024
 *
 * 	Author: Mahmoud Hashish
 */

#include "DC_Motor.h"
#include "gpio.h"
#include "PWM.h"
#include <avr/io.h>

void DcMotor_Init(void) {
	/* Initializes the DC motor by setting the direction for the motor pins and stopping the motor at the beginning */
	GPIO_setupPinDirection(PORTD_ID, PIN6_ID, LOGIC_HIGH);
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, LOGIC_HIGH);
	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
	GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state, uint8 motor_speed) {
	/* Controls the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based on the input duty cycle */

	uint8 Duty_cycle = (motor_speed * 255) / 100;
	PWM_Timer0_Start(Duty_cycle);

	if (state == CW) {
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_HIGH);
		GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);
	}

	else if (state == ACW) {
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_HIGH);
	}

	else if (state == STOP) {
		GPIO_writePin(PORTD_ID, PIN6_ID, LOGIC_LOW);
		GPIO_writePin(PORTD_ID, PIN7_ID, LOGIC_LOW);
	}
}
