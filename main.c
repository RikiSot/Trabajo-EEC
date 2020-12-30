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
  char ppm_string[9];
  int datos_flag;
  int ppm_anterior=0;
  int peligro_flag, peligro_flag_ant;

  //para debugging. Eliminar al final
  datos_flag=1;
  peligro_flag_ant=0;
  PPM=70;

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
  //init_BT();
  lcdi2cinit();
  BEEP_Init();

  //sd_init();
  //sd_estructura();
  //initFAT();


  //carga config para leer el primer sector del fichero
  //filedetected=encontrar_fichero();

  // Comprobar si existe el fichero y dar error si no existe
  // if(!filedetected)
  // {
  //
  //   LCD_command(_CLEAR_DISPLAY);
  //   LCD_cursor_at(0,0);
  //   char errorfichero[]="File not found";
  //   LCD_write(errorfichero);
  //
  //   while(1)
  //   {
  //     //softbrick
  //   }
  // }

  //2. Menu bienvenida
  initmenu();
  while(input(BTOK))
  {
    delay_ms(50);
  }
  while(!input(BTOK)) // para evitar rebotes y finalizar el programa
  {
    delay_ms(50);
  }
  //Inicializar algoritmo, adc y timer2 a 250 Hz
  adcinit();
  setup_timer2(TMR_INTERNAL | TMR_DIV_BY_64, 6000); // configurar según DELAY (10 Hz para debug)
  init_algoritmo();

  //Mostrar pantalla de frecuencia
  LCD_command(_CLEAR_DISPLAY);
  display_frecuencia();

  delay_ms(4); // que al menos se ejecute el algoritmo una vez para no tener valores raros

  //3. Bucle del programa
  while(input(BTOK))
  {
    //3.1 Ejecutar algoritmo cuando se active la flag (ISR)


    //3.2 Generar alarmas si es necesario
    if(PPM>=250 || PPM<=35)
    {
      if(peligro_flag_ant==0)
      {
        LCD_command(_CLEAR_DISPLAY);
        LCD_cursor_at(0,0);
        char peligro[]="PULSACIONES ANOMALAS";
        LCD_write(peligro);
      }
      peligro_flag=1;
    }
    else
    {
      if(peligro_flag_ant)
      {
        LCD_command(_CLEAR_DISPLAY);
        display_frecuencia();
      }
      peligro_flag=0;
    }

    //3.3 Enviar datos y escribir cada x segundos
    if(datos_flag)
    {
      //enviar_datos();
      //escribir_sd();

      //mostrar por pantalla
      if(peligro_flag==0)
      {
        if(PPM!=ppm_anterior)
        {
          sprintf(ppm_string,"%d  ",PPM);
          LCD_cursor_at(0,16);
          LCD_write(ppm_string);
        }
      }
    }
    //fin del bucle
    peligro_flag_ant=peligro_flag;
    ppm_anterior=PPM;
  }

  LCD_command(_CLEAR_DISPLAY);
  LCD_cursor_at(0,0);
  char fin[]="Fin del programa";
  LCD_write(fin);
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
