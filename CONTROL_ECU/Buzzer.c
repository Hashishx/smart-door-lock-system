/*
 *  Module: Buzzer
 *
 *  Created on: Oct 4, 2024
 *
 *  Author: Mahmoud Hashish
 */

#include "Buzzer.h"

void Buzzer_init(void) {
	/* Initializes the buzzer pin direction and turns off the buzzer */
	GPIO_setupPinDirection(PORTC_ID, PIN7_ID, LOGIC_HIGH);
	GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
}
void Buzzer_on(void) {
	/* Activates the buzzer */
	GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_HIGH);
}
void Buzzer_off(void) {
	/* Deactivates the buzzer */
	GPIO_writePin(PORTC_ID, PIN7_ID, LOGIC_LOW);
}
