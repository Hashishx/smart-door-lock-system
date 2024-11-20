/*
 * Module: PIR
 *
 *  Created on: Oct 25, 2024
 *
 *  Author: Mahmoud Hashish
 */

#include "PIR.h"
#include "gpio.h"

void PIR_init(void) {
	GPIO_setupPinDirection(PORTC_ID, PIN2_ID, PIN_OUTPUT);
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_LOW);
}

uint8 PIR_getState(void) {
	uint8 state;
	if (GPIO_readPin(PORTC_ID, PIN2_ID) == LOGIC_HIGH)
		state = LOGIC_HIGH;
	else
		state = LOGIC_LOW;
	return state;
}
