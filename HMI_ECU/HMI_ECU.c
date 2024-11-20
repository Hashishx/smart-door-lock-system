/*
 *  Project: HMI_ECU
 *
 *  Created on: Oct 26, 2024
 *
 *  Author: Mahmoud Hashish
 */
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include <util/delay.h>
//#include "Timer.h"

uint8 key, pass1[5], pass2[5], new_pass[5], flag1 = 0, flag2, tick = 0;
uint8 check_new_pass, open_door = 1, changing_pass = 0, error_Counter = 0;
uint8 wait_For_Error = 0, error_Detected = 1, PIR_State;
/*
Timer_ConfigType Timer_ConfigType_1 = { 0, 31250, TIMER1,F_CPU_256 ,COMP};

void buzzer_Timer(void) {
	tick++;
	if (tick == 60) {
		tick = 0;
		Timer_deInit(TIMER1);
	}
}

void door_Unlock_Timer(void) {
	tick++;
	if (tick == 15) {
		tick = 0;
		Timer_deInit(TIMER1);
	}
}

void door_Lock_Timer(void) {
	tick++;
	if (tick == 15) {
		tick = 0;
		Timer_deInit(TIMER1);
	}
}
*/

/* Function to get 2 passwords from user */
void get_Passwords(void) {
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1, 0);
	for (int i = 0; i < 5; i++) {
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();
		pass1[i] = key;
		if ((key <= 9) && (key >= 0)) {
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
		} else {
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
		}
		_delay_ms(500); /* Press time */
	}
	key = KEYPAD_getPressedKey();
	if (key == 13) {
		for (int k = 0; k < 5; k++) {
			UART_sendByte(pass1[k]);
		}
	}
	_delay_ms(10);
	LCD_clearScreen();
	LCD_displayString("pass1 received");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("Plz re-enter the");
	LCD_moveCursor(1, 0);
	LCD_displayString("same pass: ");

	for (int i = 0; i < 5; i++) {
		/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
		key = KEYPAD_getPressedKey();
		pass2[i] = key;
		if ((key <= 9) && (key >= 0)) {
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
		} else {
			LCD_displayCharacter('*'); /* display the pressed keypad switch */
		}
		_delay_ms(500); /* Press time */
	}
	key = KEYPAD_getPressedKey();
	if (key == 13) {
		for (int k = 0; k < 5; k++) {
			UART_sendByte(pass2[k]);
		}
	}
	_delay_ms(10);
	LCD_clearScreen();
	LCD_displayString("pass2 received");
	_delay_ms(1000);
	LCD_clearScreen();
}
int main() {

	LCD_init();
	UART_ConfigType UART_Config = {eight_bit, disabled, one_bit, 9600};
	UART_init(&UART_Config);
	while (1) {

		get_Passwords();
		/* Receive the result of comparison between the 2 passwords from the CONTROL_ECU */
		flag1 = UART_recieveByte();
		/* Proceed the application in case 2 passwords are equal */
		while (flag1 == 1) {

			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1, 0);
			LCD_displayString("- : Change Pass");
			key = KEYPAD_getPressedKey();
			_delay_ms(500); /* Press time */
			if (key == '+') {
				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);
				for (int i = 0; i < 5; i++) {
					/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
					key = KEYPAD_getPressedKey();
					new_pass[i] = key;
					if ((key <= 9) && (key >= 0)) {
						LCD_displayCharacter('*'); /* display the pressed keypad switch */
					} else {
						LCD_displayCharacter('*'); /* display the pressed keypad switch */
					}
					_delay_ms(500); /* Press time */
				}
				/* send the new password */
				key = KEYPAD_getPressedKey();
				if (key == 13) {
					for (int k = 0; k < 5; k++) {
						UART_sendByte(new_pass[k]);
					}
				}
				/* if the new password is correct, proceed the application */
				check_new_pass = UART_recieveByte();
				if (check_new_pass == 1) {
					LCD_clearScreen();
					LCD_displayString("Correct Password");
					_delay_ms(1000);
					LCD_clearScreen();
					UART_sendByte(open_door); /* Send the action needed to be taken to the CONTROL_ECU */
					LCD_displayString("Unlocking Door");
					//Timer_init(&Timer_ConfigType_1);
					//Timer_setCallBack(door_Unlock_Timer,TIMER1);
					_delay_ms(150000);
					LCD_clearScreen();
					_delay_ms(10);
					/* Recieve the PIR state form CONTROL_ECU */
					PIR_State = UART_recieveByte();
					if (PIR_State == 0) {}
					while (PIR_State == 1) {
						PIR_State = UART_recieveByte();
						if (PIR_State == 0)
							break;
						LCD_displayString("Wait for People");
						LCD_displayStringRowColumn(1,4,"to Enter    ");
					}
					LCD_clearScreen();
					LCD_displayString("Locking Door");
					//Timer_init(&Timer_ConfigType_1);
					//Timer_setCallBack(door_Lock_Timer,TIMER1);
					_delay_ms(150000);
					LCD_clearScreen();
				}
				else if (check_new_pass == 0) {
					LCD_clearScreen();
					LCD_displayString("Wrong Password");
					_delay_ms(1000);
					LCD_clearScreen();
					error_Counter++;
					if (error_Counter != 3) {
						UART_sendByte(wait_For_Error);
					}
					else if (error_Counter == 3) {
						UART_sendByte(error_Detected);
						LCD_displayString("System Locked");
						LCD_displayStringRowColumn(1,0,"Wait for 1 min");
						//Timer_init(&Timer_ConfigType_1);
						//Timer_setCallBack(buzzer_Timer,TIMER1);
						_delay_ms(150000);
						LCD_clearScreen();
						error_Counter = 0;
					}
				}
			}
			else if (key == '-') {
				LCD_clearScreen();
				LCD_displayString("Plz enter old");
				LCD_moveCursor(1, 0);
				LCD_displayString("pass:");
				for (int i = 0; i < 5; i++) {
					/* Get the pressed key number, if any switch pressed for more than 500 ms it will considered more than one press */
					key = KEYPAD_getPressedKey();
					new_pass[i] = key;
					if ((key <= 9) && (key >= 0)) {
						LCD_displayCharacter('*'); /* display the pressed keypad switch */
					} else {
						LCD_displayCharacter('*'); /* display the pressed keypad switch */
					}
					_delay_ms(500); /* Press time */
				}
				key = KEYPAD_getPressedKey();
				if (key == 13) {
					for (int k = 0; k < 5; k++) {
						UART_sendByte(new_pass[k]);
					}
				}
				/* if the new password is correct, proceed the application */
				check_new_pass = UART_recieveByte();
				if (check_new_pass == 1) {
					LCD_clearScreen();
					LCD_displayString("Correct Password");
					_delay_ms(1000);
					LCD_clearScreen();
					/* Send the action needed to be taken to the CONTROL_ECU */
					UART_sendByte(changing_pass);
					get_Passwords();
					flag2 = UART_recieveByte();
					if (flag2 == 1) {
						LCD_displayString("Password changed");
						LCD_displayStringRowColumn(1,2, "Successfully");
						_delay_ms(1000);
						LCD_clearScreen();
					}
				}
				else if (check_new_pass == 0) {
					LCD_clearScreen();
					LCD_displayString("Wrong Password");
					_delay_ms(1000);
					LCD_clearScreen();
					error_Counter++;
					if (error_Counter != 3) {
						_delay_ms(10);
						UART_sendByte(wait_For_Error);
					}
					else if (error_Counter == 3) {
						_delay_ms(10);
						UART_sendByte(error_Detected);
						LCD_displayString("System Locked");
						LCD_displayStringRowColumn(1, 0, "Wait for 1 min");
						_delay_ms(150000);
						LCD_clearScreen();
						error_Counter = 0;
					}
				}
			}
		}
		/* DO NOT Proceed the application in case 2 passwords are not equal */
		if (flag1 == 0) {
			LCD_displayString("Wrong Entry!!");
			LCD_displayStringRowColumn(1,0,"Plz try again");
			_delay_ms(1000);
			LCD_clearScreen();
		}
	}
}
