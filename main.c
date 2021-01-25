/**
 @file    main.c
 @author  Ricardo Gomez

 @brief   main EEC
*/

/* SECTION 1: Included header files to compile this file           */
#include "33FJ128MC804.h"

#include <common.h>

unsigned int16 ppm;
int1 datos_flag;
char cadena[32];
int jj;

#INT_TIMER2

void timer2_isr()
{
	ppm=algoritmo();
	//sprintf(cadena,"%04d\n",ppm); //Escribe ppm en el archivo LATIDOS
	// output_toggle(H1);
	//
	// for(jj=0;jj<5;jj++)
	// {
	// 	dt[jj+(k*5)] = cadena[jj];
	// }
	// k++;
}

#INT_TIMER5
void timer5_isr()
{
	datos_flag=1;
}

void main(void)
{
  //Declaración e inicialización de variables

  char ppm_string[9];

	// Flags del programa
  int1 peligro_flag, peligro_flag_ant, altas, bajas;

  unsigned int16 ppm_anterior=0;


	peligro_flag_ant=0;
	altas=0;
	bajas=0;

  //para debugging. Eliminar al final
  datos_flag=1;


  // -----------------------------------------

  // 1. Inicialización de módulos (BT, LCD)
  // 2. Menu de bienvenida y espera a BTOK
	// 3. Inicializar ADC y rutina del algoritmo
  // 4. Bucle de programa. Finalizar al pulsar BTOK
	//			4.1 Un ISR temporizado calcula las ppm
  //      4.2 Generar alarmas si es necesario
  //      4.3 Mostrar por pantalla
  //      4.4 Envío de datos


  //1.
  //init_BT();
  lcdi2cinit();
  BEEP_Init();
  //sd_init();

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


  //3. Inicializar algoritmo, adc y timer2 a 250 Hz
  adcinit();
  setup_timer2(TMR_INTERNAL | TMR_DIV_BY_8, 1843); // 7370000/2/8/1843 = 4 ms
	setup_timer5(TMR_INTERNAL | TMR_DIV_BY_8, 1843); // 7370000/2/8/1843 = 4 ms
  init_algoritmo();

  //Mostrar pantalla de frecuencia
  display_frecuencia();
  delay_ms(4); // que al menos se ejecute el algoritmo una vez para no tener valores raros

  //4. Bucle del programa
  while(input(BTOK))
  {

    //4.1 Generar alarmas si es necesario
    if(ppm>=250 || ppm<=35)
    {
      if(peligro_flag_ant==0) // si no ha habido peligro antes hay que actualizar la pantalla y el buzzer
      {
				if(ppm>=250) // caso alto
				{
					if(!altas) // Solo actualizar pantalla si corresponde
					{
						display_alarma_alto();
						altas=1;
						bajas=0;
						ON(BUZZER);
					}
				}
				if(ppm<=35) // caso bajo
				{
					if(!bajas)
					{
						display_alarma_bajo();
						bajas=1;
						altas=0;
						ON(BUZZER);
					}
				}
      }
      peligro_flag=1;

    }//fin bloque de alarmas


		//4.2 Mostrar por pantalla en funcionamiento normal
    else
    {
      if(peligro_flag_ant) // si antes ha habido peligro pero ahora no. hay que actualizar pantalla y apagar el buzzer
      {
        display_frecuencia();
				OFF(BUZZER);
      }
      peligro_flag=0; // actualizar flag
			bajas=0;
			altas=0;

			if(ppm!=ppm_anterior) // mostrar por pantalla si ha cambiado el valor
			{
				sprintf(ppm_string,"%d  ",ppm); // crear cadena con espacios para borrar el valor anterior
				LCD_cursor_at(0,16);
				LCD_write(ppm_string);
				ppm_anterior=ppm; 							// actualizar pulsaciones anteriores
			}
    }

    //4.3 Enviar datos y escribir cada x segundos
    if(datos_flag)
    {
      enviar_datos(ppm);
			
			//sd_init_global();
			//escritura_sd();
			datos_flag=0;
		}

    //fin del bucle
    peligro_flag_ant=peligro_flag;
  }

	//fin de programa
	display_final();
}
