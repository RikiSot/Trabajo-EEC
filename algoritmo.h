/**
 @file    ALGORITMO.h
 @author  Ricardo Gomez, Klaudija Ziliute
 @brief   PanTompkins header file
*/


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
