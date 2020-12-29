/**
 @file    BEEP.h
 @author  Julio Gomis-Tena
 @brief   Generate n "beeps" of variable frequency
*/
// Do not write above this line (except comments)!

#ifndef __ALGORITMO_H
#define __ALGORITMO_H

#include "common.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "adc.h"
#include "BEEP.h"

int PPM; // variable global

void algoritmo();
void init_algoritmo(void);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // __ALGORITMO_H
