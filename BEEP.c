#include "BEEP.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BEEP_Init(unsigned int16 freq)		//4kHz -> T/2= 125us
{
	unsigned int16 fbeeps, presettimer;
	fbeeps=70*freq-2620;
	presettimer=5529600/fbeeps;
	setup_timer3(TMR_INTERNAL|TMR_DIV_BY_1, presettimer); // Frecuencia de los beeps variable con frecuencia
	set_compare_time(2, 0);
	setup_compare(2, COMPARE_OFF);

		// (11059200/2)/256/432 = 50 Hz  <-> 100ms
	setup_timer4(TMR_INTERNAL|TMR_DIV_BY_256, 2160);
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
