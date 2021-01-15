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


#endif // __LCD_H
// Do not write below this line!
