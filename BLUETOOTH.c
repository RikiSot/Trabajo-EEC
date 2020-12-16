#include "BLUETOOTH.h"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 cnt, InBuff[256]; // Data will be stored in InBuff

#int_rda
void uart_isr(void)
{
	while(kbhit(BT)) // if its ready
	{
		InBuff[cnt++]=fgetc(BT);
	};
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//extern unsigned int8 ADR[6];
extern char BT_Adrs[14];

void initBT(void)
{
  memset(InBuff,0xFF,256);
  disable_interrupts(INT_RDA);
  clear_interrupt(INT_RDA);

  delay_ms(1000);
  fprintf(BT,"$$$");	// enter command mode	Rsp = 'C' 'M' 'D' \r \n
	cnt=0; memset(InBuff,0xFF,256);
	do{InBuff[cnt++]=fgetc(BT);}while(InBuff[cnt-1]!=0x0A);

  fprintf(BT,"SF,1\r"); // to factory defaults  Rsp = 'A' 'O' 'K' \r \n
	cnt=0; memset(InBuff,0xFF,256);
	do{InBuff[cnt++]=fgetc(BT);}while(InBuff[cnt-1]!=0x0A);

  fprintf(BT,"GB\r");   // BT Device Address
	cnt=0; memset(InBuff,0xFF,256);
	do{InBuff[cnt++]=fgetc(BT);}while(InBuff[cnt-1]!=0x0A);
	memset(BT_Adrs,0,14);
	memcpy(BT_Adrs,InBuff,12);

  fprintf(BT,"SA,0\r"); // set 'Open mode'		Rsp = 'A' 'O' 'K' \r \n
	cnt=0; memset(InBuff,0xFF,256);
	do{InBuff[cnt++]=fgetc(BT);}while(InBuff[cnt-1]!=0x0A);

  fprintf(BT,"R,1\r");  // forces a reboot 		Rsp = 'R' 'e' 'b'  'o' 'o' 't'  '!' \r \n
	cnt=0; memset(InBuff,0xFF,256);
	do{InBuff[cnt++]=fgetc(BT);}while(InBuff[cnt-1]!=0x0A);

  cnt=0;
  enable_interrupts(INT_RDA);
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
