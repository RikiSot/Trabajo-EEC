#include "BLUETOOTH.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
float phase = 0;
unsigned int8 snd;

void init_BT(void)
{
  	fprintf(BT,"$$$");	     delay_ms(500); // command mode
  	fprintf(BT,"SF,1\r");      delay_ms(500); // factory defaults
  	fprintf(BT,"SA,0\r");      delay_ms(500); // authentication 'Open'
  	fprintf(BT,"R,1\r");       delay_ms(500); // Run device
}

void enviar_datos()
{
	COM:
	while(!input(COMM));
		fprintf(BT,"$Representacion grafica,Funcion,datos 1\n");
 		fprintf(BT,"&0,1000,0,1,0\n");
 		fprintf(BT,"#0,200,0,1,0\n");

	while(1)
	{
 		phase= (phase < 2*3.1416) ? (phase + 0.01) : 0;
 		snd = (unsigned int8)(100+50*sin(phase)+30*sin(2*phase+0.5)-20*sin(7*phase+1));
 		fprintf(BT, "%3u\n", snd);
		delay_ms(10);
		if(!input(COMM))
		goto COM;
	}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
