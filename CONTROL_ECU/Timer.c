/*
 *  Module: Timer
 *
 *  Created on: Oct 26, 2024
 *
 *  Author: Mahmoud Hashish
 */

#include "Timer.h"
#include <avr/io.h>
#include "gpio.h"
#include <avr/interrupt.h>
#include "common_macros.h"
/* Global variable to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR(TIMER0_OVF_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect) {
	if(g_callBackPtr != NULL_PTR) {
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Timer_init(const Timer_ConfigType * Config_Ptr) {

	if (Config_Ptr->timer_ID == Timer_0) {

		TCNT0 = Config_Ptr->timer_InitialValue;
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock);
		if (Config_Ptr->timer_mode == Normal_Mode) {
			CLEAR_BIT(TCCR0,WGM00);
			CLEAR_BIT(TCCR0,WGM01);
			SET_BIT(TCCR0,FOC0);
			SET_BIT(TIMSK, TOIE0);
		}
		else if (Config_Ptr->timer_mode == CTC_Mode){
			CLEAR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
			SET_BIT(TCCR0,FOC0);
			SET_BIT(TIMSK, OCIE0);
			OCR0 = Config_Ptr->timer_compare_MatchValue;
		}
	}
	else if(Config_Ptr->timer_ID == Timer_1) {

		TCNT1 = Config_Ptr->timer_InitialValue;
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock);
		if (Config_Ptr->timer_mode == Normal_Mode) {
			SET_BIT(TCCR1A,FOC1A);
			SET_BIT(TCCR1A,FOC1B);
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			CLEAR_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			SET_BIT(TIMSK, TOIE1);
		}
		else if (Config_Ptr->timer_mode == CTC_Mode) {
			SET_BIT(TCCR1A,FOC1A);
			SET_BIT(TCCR1A,FOC1B);
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1A,WGM11);
			SET_BIT(TCCR1B,WGM12);
			CLEAR_BIT(TCCR1B,WGM13);
			SET_BIT(TIMSK, OCIE1A);
			OCR1A = Config_Ptr->timer_compare_MatchValue;
		}
	}
	else if (Config_Ptr->timer_ID == Timer_2) {

		TCNT2 = Config_Ptr->timer_InitialValue;
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock);
		if (Config_Ptr->timer_mode == Normal_Mode) {
			CLEAR_BIT(TCCR2,WGM20);
			CLEAR_BIT(TCCR2,WGM21);
			SET_BIT(TCCR2,FOC2);
			SET_BIT(TIMSK, TOIE2);
		}
		else if (Config_Ptr->timer_mode == CTC_Mode) {
			CLEAR_BIT(TCCR2, WGM20);
			SET_BIT(TCCR2, WGM21);
			SET_BIT(TCCR2, FOC2);
			SET_BIT(TIMSK, OCIE2);
			OCR2 = Config_Ptr->timer_compare_MatchValue;
		}
	}
}

void Timer_deInit(Timer_ID_Type timer_type) {

	g_callBackPtr = NULL_PTR;
	if (timer_type == Timer_0) {
		TCNT0 = 0;
		OCR0 = 0;
	} else if (timer_type == Timer_1) {
		TCCR1B = 0;
		TCNT1 = 0;
	} else if (timer_type == Timer_2) {
		TCCR2 = 0;
		TCNT2 = 0;
	}
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID) {

	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
