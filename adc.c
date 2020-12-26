/**
 @file    adc.c
 @author  Ricardo Gomez

 @brief   adc functions
*/
// Do not write above this line (except comments)!

#include "adc.h"
#include "common.h"

#INT_ADC1


void isr_adc(void) // pasa el valor del BUF a una variable global
{
  //adcflag=1; no funciona la flag???
  output_toggle(PIN_B12); // lectura actualizada
}



void adcinit(void) //por comprobar en la placa
{

  setup_timer3(TMR_INTERNAL, 2303); //  TCLK = (1/7.37) x 2 x 2303 = 625 us (=1/1600Hz) ojo porque en el adc Tosc entra x2  //tsmp=samc<4:0>*tad

  ADC1CON2 = 0b0000000000000000; // voltage reference en ADC1CON2
  ADC1CON3 = 0b1000000000000000; // select the analog conversion clock <7:0> y <15> para enable Tad
//  AD1PCFGL = 0b1111111111101111; // AN4 entrada analogica - RB1 - PGC en placa
//  AD1CHS0  = 0b0000010000000100; // AN4 entrada seleccionada para el CH0
  AD1PCFGL = 0b0000000011110111; // AN4 entrada analogica - RB1 - PGC en placa
  AD1CHS0  = 0b0000010000000100; // AN4 entrada seleccionada para el CH0

  ADC1CON1 = 0b1011010101000110; // 12 bits - Signed Integer - <7:5> 010 Timer3 for ADC1 - <15> Sampling inmediatly after EOC <9:0> unsigned int

  enable_interrupts(INT_ADC1);

}
