/**
 @file    SD_CARD.h
 @author  Ricardo Gomez, Klaudija Ziliute
 @brief   PanTompkins header file
*/


#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "common.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "miFAT.h"
#include "sdcard.h"

void sd_init_global();
void escritura_sd();
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif // __SD_CARD_H
