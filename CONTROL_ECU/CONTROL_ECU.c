/*
 *  Project: CONTROL_ECU
 *
 *  Created on: Oct 26, 2024
 *
 *  Author: Mahmoud Hashish
 */

#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include <util/delay.h>
#include "Buzzer.h"
#include "DC_Motor.h"
#include "PIR.h"
/*
#include "Timer.h"

Timer_ConfigType Timer_ConfigType_1 = { 0, 31250, TIMER1,F_CPU_256 ,COMP};
uint8 tick = 0;
void buzzer_Timer(void) {
	tick++;
	if (tick == 60) {
		tick = 0;
		Timer_deInit(TIMER1);
		Buzzer_off();
	}
}

void door_Unlock_Timer(void) {
	tick++;
	if (tick == 15) {
		tick = 0;
		Timer_deInit(TIMER1);
		DcMotor_Rotate(STOP, 100);
	}
}

void door_Lock_Timer(void) {
	tick++;
	if (tick == 15) {
		tick = 0;
		Timer_deInit(TIMER1);
		DcMotor_Rotate(STOP, 100);
	}
}
*/
int main() {

	uint8 flag = 0, Action, flag1 = 0, PIR_State, flag2 = 0;
	uint8 pass1[5], pass2[5], new_pass[5], saved_pass[5];
	uint8 wrong_pass = 0, correct_pass = 1, error_Check;
	uint8 object_Detected = 1, no_Object_Detected = 0;
	UART_ConfigType UART_Config = {eight_bit, disabled, one_bit, 9600};
	TWI_ConfigType TWI_Config = {2, F_CPU_1};

	UART_init(&UART_Config);
	TWI_init(&TWI_Config);
	Buzzer_init();
	DcMotor_Init();
	PIR_init();
	while(1) {
		/* Receive the first password from HMI_ECU */
		for (int i = 0; i < 5; i++) {
			pass1[i] = UART_recieveByte();
		}
		_delay_ms(10);
		/* Receive the second password from HMI_ECU */
		for (int i = 0; i < 5; i++) {
			pass2[i] = UART_recieveByte();
		}
		_delay_ms(10);
		/* Compare the 2 passwords */
		for (int j = 0; j < 5; j++) {
			if (pass1[j] != pass2[j]) {
				flag1 = 0;
				break;
			}
			else if (pass1[j] == pass2[j]) {}
			flag1 = 1;
		}
		/* Send the comparison result to the HMI_ECU */
		UART_sendByte(flag1);
		/* Save the system password in the EEPROM in case the 2 passwords are equal */
		if (flag1 == 1) {
			for (int j = 0; j < 5; j++) {
				EEPROM_writeByte((0x0311 + j), pass1[j]);
				_delay_ms(10);
				EEPROM_readByte((0x0311 + j), &(saved_pass[j]));
			}
		}
		/* if the password is correct, proceed the application */
		while (flag1 == 1) {
			/* recieve the new password */
			for (int k = 0; k < 5; k++) {
				new_pass[k] = UART_recieveByte();
			}
			/* check if the new password is correct or not with the EEPROM */
			for (int i = 0; i < 5; i++) {
				if (new_pass[i] != saved_pass[i]) {
					flag = 0;
					break;
				}
				else if (new_pass[i] == saved_pass[i]) {}
				flag = 1;
			}
			/* if the new password is correct, proceed the application */
			if (flag == 1) {
				/* Send a byte to the HMI_ECU to let it know whether the new password is correct or not */
				UART_sendByte(correct_pass);
				_delay_ms(1000);
				/* Recieve the action needed to be done from the HMI_ECU */
				Action = UART_recieveByte();
				_delay_ms(10);
				/* Opening door */
				if (Action == 1) {
					DcMotor_Rotate(CW, 100);
					_delay_ms(150000);
					//Timer_init(&Timer_ConfigType_1);
					//Timer_setCallBack(door_Unlock_Timer,TIMER1);
					DcMotor_Rotate(STOP, 100);
					PIR_State = PIR_getState(); /* check PIR state */
					if (PIR_State == 1)
						UART_sendByte(object_Detected);
					else if (PIR_State == 0)
						UART_sendByte(no_Object_Detected);
					while (PIR_State == 1) {
						PIR_State = PIR_getState();
						if (PIR_State == 0) {
							UART_sendByte(no_Object_Detected);
							break;
						}
						UART_sendByte(object_Detected);
						DcMotor_Rotate(CW, 100);
					}
					DcMotor_Rotate(ACW, 100);
					_delay_ms(150000);
					//Timer_init(&Timer_ConfigType_1);
					//Timer_setCallBack(door_Lock_Timer,TIMER1);
					DcMotor_Rotate(STOP, 100);
				}
				/* Changing Password */
				else if (Action == 0) {
					/* Receive the first password from HMI_ECU */
					for (int i = 0; i < 5; i++) {
						pass1[i] = UART_recieveByte();
					}
					_delay_ms(10);
					/* Receive the second password from HMI_ECU */
					for (int i = 0; i < 5; i++) {
						pass2[i] = UART_recieveByte();
					}
					_delay_ms(10);
					/* Compare the 2 passwords */
					for (int j = 0; j < 5; j++) {
						if (pass1[j] != pass2[j]) {
							flag2 = 0;
							break;
						} else if (pass1[j] == pass2[j]) {
						}
						flag2 = 1;
					}
					/* Save the system password in the EEPROM in case the 2 passwords are equal */
					if (flag2 == 1) {
						for (int j = 0; j < 5; j++) {
							EEPROM_writeByte((0x0311 + j), pass1[j]);
							_delay_ms(10);
							EEPROM_readByte((0x0311 + j), &(saved_pass[j]));
						}
					}
					UART_sendByte(flag2);
				}
			}
			else if (flag == 0) {
				/* Send a byte to the HMI_ECU to let it know whether the new password is correct or not */
				UART_sendByte(wrong_pass);
				_delay_ms(1000);
				error_Check = UART_recieveByte();
				/* if the password is not correct for 3 consecutive times, warning system triggers  */
				if (error_Check == 0){}
				else if (error_Check == 1) {
					Buzzer_on();
					_delay_ms(150000);
					//Timer_init(&Timer_ConfigType_1);
					//Timer_setCallBack(buzzer_Timer,TIMER1);
					Buzzer_off();
				}
			}
		}
	}
}
