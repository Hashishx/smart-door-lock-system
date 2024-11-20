/*
 *  Module: PWM
 *
 *  Created on: Oct 5, 2024
 *
 *  Author: Mahmoud Hashish
 */

#include "PWM.h"
#include "common_macros.h"
#include <avr/io.h>
#include "gpio.h"

/* Initializes Timer0 in PWM mode and sets the required duty cycle.
 * Prescaler: F_CPU/64
 * Non-inverting mode
 * The function configures OC0 as the output pin. */
void PWM_Timer0_Start(uint8 duty_cycle) {

	TCNT0 = 0;

	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,COM01);
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS01);

	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);

	OCR0 = duty_cycle;
}

