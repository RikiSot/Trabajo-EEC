/**
 @file    adc.h
 @author  Ricardo Gomez
 @brief   adc init and ISR
*/
// Do not write above this line (except comments)!

#ifndef __ADC_H
#define __ADC_H

#include "common.h"

#WORD ADC1BUF0 = 0x0300;
#WORD ADC1CON1 = 0x0320;
#WORD ADC1CON2 = 0x0322;
#WORD ADC1CON3 = 0x0324;
#WORD AD1CHS0 = 0x0328;
#WORD AD1PCFGL = 0x032C;
#WORD IFS0 = 0x0084;

int adcbuffer[30]; // para cumplir con los 150 ms del articulo
int adcbuffer2[30];

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void adcinit(void);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // __ADC_H
