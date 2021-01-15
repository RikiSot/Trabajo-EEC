#ifndef Bluetooth_h
#define Bluetooth_h
#include "Header.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#use rs232(UART1,baud=115200,STREAM=BT)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void init_BT(void); //inits and enables uart isr
void enviar_datos(unsigned int16 ppm);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
