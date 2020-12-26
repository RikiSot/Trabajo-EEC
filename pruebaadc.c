#include "lcd.h"
#include "adc.h"
#include "algoritmo.h"
#include "common.h"

void main()
{
  char adcvalor[9];
  int adcbuff;
  output_high(PIN_A8); // necesario para el potenciometro
  lcdi2cinit();
  initmenu();
  while(input(PIN_A7))
  {
    delay_ms(100);
  }
  adcinit();
  LCD_command(_CLEAR_DISPLAY);
  display_valueADC_title();
  while(1)
  {
      adcbuff=algoritmo();
      sprintf(adcvalor,"%d   ",adcbuff);
      LCD_write(adcvalor);
      LCD_cursor_at(0,11);
  }
}
//comentario
