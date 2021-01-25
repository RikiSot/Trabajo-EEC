/**
 @file    SD_CARD.h
 @author  Ricardo Gomez, Klaudija Ziliute
 @brief   PanTompkins header file
*/


#ifndef __SD_CARD_H
#define __SD_CARD_H

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "common.h"
#include "miFAT.h"
#include "sdcard.h"

void sd_init_global();
void escritura_sd();
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//Inicialización de la SD
void sd_init_global(){
  set_tris_b(0b1111111110011111);
  set_tris_c(0b1111111111011111);
  delay_ms(30);

  sd_init();	// Inicializa micro SD

  initFAT();	// Carga los parametros del sistema de ficheros

}

//Escritura en el fichero LATIDOS
void escritura_sd(){
  inicializa_fichero();
  enable_interrupts(INT_TIMER2);
  memset(dt,0,512);
  k = 0;
  OFF(H2);

  while(k<15);
  disable_interrupts(INT_TIMER2);
  escribe_datos_en_fichero();
  for(;;)
  {
    ON(H2);
  }
}

#endif // __SD_CARD_H
