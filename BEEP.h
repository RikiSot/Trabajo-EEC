/**
 @file    BEEP.h
 @author  Julio Gomis-Tena
 @brief   Generate n "beeps" of variable frequency
*/
// Do not write above this line (except comments)!

#ifndef __BEEP_H
#define __BEEP_H
#include "Header.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#pin_select OC2 = BUZZER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BEEP_Init(void); //Inicializa las frecuencias de los beeps y la separación entre ellos
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BEEP(unsigned int8); //Realiza n beeps
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BEEP_Init(void)		//4kHz -> T/2= 125us
{
	setup_timer3(TMR_INTERNAL|TMR_DIV_BY_1, 737); // 5kHz. Frecuencia de los beeps
	set_compare_time(2, 0);
	setup_compare(2, COMPARE_OFF);

		// (11059200/2)/256/432 = 50 Hz  <-> 100ms
	setup_timer4(TMR_INTERNAL|TMR_DIV_BY_8, 1843);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 N_beep;

void BEEP(unsigned int8 n)
{
	if(n<127)
		N_beep = 2*n;
	else
		N_beep = 255;

	enable_interrupts(INT_TIMER4);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#INT_TIMER4
void isr_TIMER4()			// Cada 100 ms. Espaciado entre beeps. Habra que asociarlo a una interrupcion por software
{
		if((N_beep%2)==1)
		{
			setup_compare(2, COMPARE_TOGGLE | COMPARE_TIMER3);
		}
		else
		{
			setup_compare(2, COMPARE_OFF);
			OFF(BUZZER);
		}

		if( N_beep>0 )
		{
		   	N_beep-=1;
		}
		else
		{
			disable_interrupts(INT_TIMER4);
		}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif // __BEEP_H
