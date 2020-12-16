/**
 @file    main.c
 @author  Ricardo Gomez

 @brief   main EEC
*/

/* SECTION 1: Included header files to compile this file           */

#include <common.h>

void main(void)
{
  //Declaración e inicialización de variables

  unsigned int16 analogsignal;    // Señal analógica leída del ADC
  unsigned int16 frecuencia;      // Frecuencia cardíaca
  unsigned int16 Sector;          // Sector de la SD
  unsigned int1 filedetected;

  // -----------------------------------------

  // 1. Inicialización de módulos (BT, LCD, ADC)
  //      encontrar fichero de datos anterior o crear uno nuevo
  // 2. Menu de bienvenida y espera a BTOK
  // 3. Bucle de programa. Finalizar al pulsar BTOK
  //      3.1 Muestreo de señal (ADC)
  //      3.2 Envío de los datos por bluetooth
  //      3.3 Guardar datos en SD (formato??)
  //      3.4 Cálculo de la frecuencia
  //      3.5 Display frecuencia en LCD
  //      3.6 Generar beep de acuerdo a la frecuencia (no n beeps, beep con corazón). Interrupcion cuando se encuentra pico
  //      3.7 Generar una flag si la frecuencia es X para activar interrupciones

  //1.
  initBT();
  lcdi2cinit();

  sd_init();
  sd_estructura();
  initFAT();


  //carga config para leer el primer sector del fichero
  filedetected=encontrar_fichero();

  // Comprobar si existe el fichero y dar error si no existe
  if(!filedetected)
  {
    errorfichero();
    while(1)
    {
      //softbrick
    }
  }

  //adcinit

  //2.
  initmenu();
  while(input(BTOK))
  {
    delay_ms(100);
  }

  //3.
  while(input(BTOK))
  {
    //3.1

    //3.2

    //3.3
    //dt=datos a escribir (fecha, valor); solo valor???
        //time[]
        //value[]
        //dt[]="dd/mm/aa hh:mm:ss  PPM"

    escribe_datos_en_fichero(); // escribe lo que haya en el buffer
  }
}
