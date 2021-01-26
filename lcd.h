/**
 @file    lcd.h
 @author  Ricardo G�mez
 @brief   LCD management
*/
// Do not write above this line (except comments)!

#ifndef __LCD_H
#define __LCD_H

#include "common.h"

/* SECTION 1: Included header files to compile this file           */

/* SECTION 2: Public macros                                        */

#define _CLEAR_DISPLAY 				0x01
#define _8BIT_4LINES_NORMAL_RE1_IS0		0x3A
#define _8BIT_4LINES_REVERSE_RE1_IS0		0x3B
#define _8BIT_4LINES_RE0_IS1			0x39
#define _8BIT_4LINES_RE0_IS0			0x38

#define _BS1_1					0x1E
#define _POWER_DOWN_DISABLE			0x02
#define _SEGMENT_NORMAL_WAY			0x05
#define _NW					0x09

#define _DISPLAY_ON_CURSOR_ON_BLINK_ON		0x0F
#define _DISPLAY_ON_CURSOR_ON_BLINK_OFF		0x0E
#define _DISPLAY_ON_CURSOR_OFF_BLINK_OFF	0x0C
#define _BS0_1					0x1C
#define _INTERNAL_DIVIDER			0x13
#define _CONTRAST				0x77
#define _POWER_ICON_CONTRAST			0x5C
#define _FOLLOWER_CONTROL			0x6E

#define ADDRESS_DDRAM				0x80


/* SECTION 3: Public types                                         */


/* SECTION 4: Public variables :: declarations, extern mandatory   */


/* SECTION 5: Public functions :: declarations, extern optional
   Rule exception (callbacks)  :: declarations, extern recommended */
void display_valueADC_title(void);
void LCD_command(unsigned int8);
void lcdi2cinit(void);
void LCD_write(char*);
void initmenu(void);                                      //Inicializa el menu principal
void display_frecuencia(void);       // Muestra frecuencia
void LCD_cursor_at(unsigned int8 line, unsigned int8 column);
void display_alarma_alto(void); // Limpia pantalla y escribe mensaje de alarma
void display_alarma_bajo(void);
void display_final(void);
void display_electrodo_suelto(void);
void display_electrodo_recuperado(void);


void lcdi2cinit(void)
{

  OFF(SD);
  delay_ms(200);
  ON(PIN_C1);

  //leds output en open drain
  output_float(PIN_B8);
  output_float(PIN_B9);

  //pullups
  set_pullup(TRUE, PIN_B8);
  set_pullup(TRUE, PIN_B9);


  //dspic as master, velocidad, nombre de la conexion
  #use i2c(Master, Fast=10000, i2c1)

  LCD_command(_CLEAR_DISPLAY);
  LCD_command(_8BIT_4LINES_NORMAL_RE1_IS0);
  LCD_command(_BS1_1);
  LCD_command(_POWER_DOWN_DISABLE);
  LCD_command(_SEGMENT_NORMAL_WAY);
  LCD_command(_NW);
  LCD_command(_8BIT_4LINES_RE0_IS1);
  LCD_command(_DISPLAY_ON_CURSOR_ON_BLINK_OFF);
  LCD_command(_BS0_1);
  LCD_command(_INTERNAL_DIVIDER);
  LCD_command(_CONTRAST);
  LCD_command(_POWER_ICON_CONTRAST);
  LCD_command(_FOLLOWER_CONTROL);
  LCD_command(_8BIT_4LINES_RE0_IS0);
  LCD_command(_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);

}

void LCD_command(unsigned int8 cmd)
{
  i2c_start();
  i2c_write(0x78);
  i2c_write(0x00);
  i2c_write(cmd);
  i2c_stop();
}

void LCD_write(char* cd)
{
  unsigned int8 c=0;
  i2c_start();
  i2c_write(0x78);
  i2c_write(0x40);
  while(cd[c]!=0)
  {
    i2c_write(cd[c]);
    c++;
  }
  i2c_stop();
}

void LCD_cursor_at(unsigned int8 line, unsigned int8 column)
{
  LCD_command(0x80 | (line*0x20+column));
}

void initmenu(void)
{
	char cabecera[]="----WELCOME-----";
	char boton[]="-------------";
	char palabra1[]="PRESS OK";
	//char palabra2[]="OPCION 2";
	//char palabra3[]="OPCION 3";
	//char palabra4[]="OPCION 4";

	LCD_cursor_at(0,0);
	LCD_write(cabecera);
	LCD_cursor_at(1,0);
	LCD_write(boton);
	LCD_cursor_at(2,1);
	LCD_write(palabra1);
}

void errorfichero(void)
{
	char cabecera[]="------ERROR------";
	char boton[]="NO CONFIG.TXT DETE";
	char palabra1[]="PRESS RESET";
	//char palabra2[]="OPCION 2";
	//char palabra3[]="OPCION 3";
	//char palabra4[]="OPCION 4";

	LCD_cursor_at(0,0);
	LCD_write(cabecera);
	LCD_cursor_at(1,0);
	LCD_write(boton);
	LCD_cursor_at(2,1);
	LCD_write(palabra1);
}

void display_frecuencia(void)
{
  LCD_command(_CLEAR_DISPLAY);
	char cabecera[]="FRECUENCIA PPM: ";
  char palabra1[]="-------------------";
  char palabra2[]="BTOK PARA FINALIZAR";
	LCD_cursor_at(0,0);
  LCD_write(cabecera);
  LCD_cursor_at(1,0);
  LCD_write(palabra1);
  LCD_cursor_at(2,0);
  LCD_write(palabra2);
}

void display_valueADC_title(void)
{
  char cabecera[]="VALOR ADC: ";
  //char palabra2[]="OPCION 2";
  //char palabra3[]="OPCION 3";
  //char palabra4[]="OPCION 4";

  LCD_cursor_at(0,0);
  LCD_write(cabecera);
}

void display_alarma_bajo(void)
{
  LCD_command(_CLEAR_DISPLAY);
  LCD_cursor_at(0,0);
  char peligro[]="PULSACIONES BAJAS";
  LCD_write(peligro);
}

void display_alarma_alto(void)
{
  LCD_command(_CLEAR_DISPLAY);
  LCD_cursor_at(0,0);
  char peligro[]="PULSACIONES ELEVADAS";
  LCD_write(peligro);
}

void display_final(void)
{
  LCD_command(_CLEAR_DISPLAY);
  LCD_cursor_at(0,0);
  char fin[]="Fin del programa";
  LCD_write(fin);
}

void display_electrodo_suelto(void)
{
  LCD_command(_CLEAR_DISPLAY);
  LCD_cursor_at(0,0);
  char fin[]="ELECTRODO SUELTO";
  LCD_write(fin);
}

void display_electrodo_recuperado(void)
{
  LCD_command(_CLEAR_DISPLAY);
	char cabecera[]="Contacto restaurado";
  char palabra1[]="-------------------";
  char palabra2[]="BTOK PARA CONTINUAR";
	LCD_cursor_at(0,0);
  LCD_write(cabecera);
  LCD_cursor_at(1,0);
  LCD_write(palabra1);
  LCD_cursor_at(2,0);
  LCD_write(palabra2);
}


#endif // __LCD_H
// Do not write below this line!
