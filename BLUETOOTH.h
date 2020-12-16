#ifndef Bluetooth_h
#define Bluetooth_h
#include "Header.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#use rs232(UART1,baud=115200,STREAM=BT,errors)
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void initBT(void); //inits and enables uart isr
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
