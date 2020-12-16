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
#endif // __BEEP_H
