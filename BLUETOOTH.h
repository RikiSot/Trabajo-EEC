#ifndef Bluetooth_h
#define Bluetooth_h
#include "Header.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#use rs232(UART1,baud=115200,STREAM=BT)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void init_BT(void); //inits and enables uart isr
void enviar_datos(unsigned int16 ppm);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void init_BT(void)
{
  	fprintf(BT,"$$$");	     delay_ms(500); // command mode
  	fprintf(BT,"SF,1\r");      delay_ms(500); // factory defaults
  	fprintf(BT,"SA,0\r");      delay_ms(500); // authentication 'Open'
  	fprintf(BT,"R,1\r");       delay_ms(5000); // Run device
    fprintf(BT,"$Representacion grafica,Funcion,datos 1\n");
    fprintf(BT,"&0,250,0,1,0\n"); //X
    fprintf(BT,"#0,250,0,1,0\n"); //Y

}

void enviar_datos(unsigned int16 ppm)
{
	COM:
	while(!input(COMM));
 		fprintf(BT, "%03d\n", ppm);
		if(!input(COMM))
		goto COM;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
