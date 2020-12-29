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

  // unsigned int16 analogsignal;    // Señal analógica leída del ADC
  // unsigned int16 frecuencia;      // Frecuencia cardíaca
  // unsigned int16 Sector;          // Sector de la SD
  // unsigned int1 filedetected;
  int ppm;
  char ppm_string[9];

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
  init_BT();
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

  //2.
  initmenu();
  while(input(BTOK))
  {
    delay_ms(100);
  }

  //Inicializar algoritmo y adc
  adcinit();
  init_algoritmo();

  //Mostrar pantalla de frecuencia
  LCD_command(_CLEAR_DISPLAY);
  display_frecuencia();

  //3. Bucle del programa
  while(input(BTOK))
  {
    //3.1 Ejecutar algoritmo
    ppm=algoritmo();

    //3.2 Enviar datos
    enviar_datos();
    //3.3 escribir en sd

    //3.3 mostrar por pantalla
    if(ppm=!ppm_anterior)
    {
      sprintf(ppm_string,"%d  ",ppm);
      LCD_write(ppm_string);
      LCD_cursor_at(0,16);
      ppm_anterior=ppm;
    }

    //3.4 Generar alarmas si es necesario
    if(ppm>=250 || ppm<=35)
    {
      LCD_command(_CLEAR_DISPLAY);
      // mensaje error y alarma
    }

    //fin del bucle
  }
  //fin de programa
}


// correo para Julio

/*
preguntar si el algoritmo necesita un buffer o puede trabajar a tiempo real.
no nos muestra las pulsaciones y no lo entendemos bien
el resto esta solucionado mas o menos

amplificador del eec. saca unsigned desde 0 o simplemente el valor de mv

canal 4???
*/
