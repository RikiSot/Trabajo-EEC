#include "Header.h"
#include "SD_Card.h"
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// unsigned int8  sdhc;
// unsigned int8  dt[512];            /// Buffer de escritura  en uSD
// unsigned int32 LBA0;
//
// //variables clave: inicio de la FAT1 y la FAT2 y de las entradas de ficheros
unsigned int32 MxSize;
unsigned int16 sect_x_cluster;
unsigned int32 sect_fat_1;
unsigned int32 sect_fat_2;
unsigned int16 RootEntCnt;
unsigned int32 sect_ini_datos;
unsigned int32 sect_x_fat;
unsigned int32 sect_entry;
unsigned int16 cluster_ini;
unsigned int16 cluster;
unsigned int32 sector_ini;
unsigned int32 sector;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
inline unsigned int8 xfer_spi(char envio){
 unsigned int8 ret;
  ON( ROJO);
  ret = spi_read(envio);
  OFF(ROJO);
  return ret;
}//fin xfer_spi
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 Commnd(char CMD, int32 SD_Adress, char CRC){
   unsigned int8 iC1;

   xfer_spi(0xFF);
   xfer_spi(CMD);
   xfer_spi(make8(SD_Adress, 3));
   xfer_spi(make8(SD_Adress, 2));
   xfer_spi(make8(SD_Adress, 1));
   xfer_spi(make8(SD_Adress, 0));
   xfer_spi(CRC);

   do{iC1 = xfer_spi(0xFF);
   }while(iC1 == 0xFF);
   return iC1;
}//fin Commnd
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_init(){
  set_tris_b(0b1111111110011111);
  set_tris_c(0b1111111111011111); 
 unsigned  int8 R[17];
 unsigned  int8 versionSD= 1;
 unsigned  int8 crc;
 unsigned int32 arg=0;

 	setup_spi(SPI_MASTER|SPI_SS_DISABLED|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_DIV_16);

	do	// CMD0 - GO_IDLE_STATE  (R1)	Card Reset
	{
		output_high(CS);   	// tarjeta deshabilitada
		xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);
		xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);	xfer_spi(0xFF);
		output_low(CS); 	// tarjeta habilitada
		R[0] = Commnd( CMD0 , 0x00000000 , 0x95); //go to idle
	}while( R[0] != R1_IDLE_STATE);

  	// CMD8 - SEND_IF_COND (R7)  	Send Interface Condition Command
	R[0] = Commnd(CMD8, 0x000001AA, 0x87);
    if(R[0]& R1_ILLEGAL_COMMAND)
	{
												versionSD = 1;
   	}
	else
	{
		R[1] = xfer_spi(0xFF);
		R[2] = xfer_spi(0xFF);
		R[3] = xfer_spi(0xFF);
		R[4] = xfer_spi(0xFF);
	    if(R[4]!=0xAA){	output_high(CS); return 1; }		//error

      											versionSD = 2;
	}

  	// ACMD41 - SD_SEND_OP_ COND (R1) 	Initialization Command
   	arg = (versionSD == 2) ? 0x40000000 : 0;
   	crc = (versionSD == 2) ? 0x77 : 0xFF;
   	do
   	{   delay_ms(4);
        Commnd(CMD55,    0, 0x65);			// El proceso puede llegar a durar hasta 1 segundo
        R[0] = Commnd(ACMD41, arg, crc);	// Application-Specific Command � APP_CMD (CMD55)
   	}while(R[0] != R1_READY_STATE);

  if(versionSD == 2)
  {
  	// CMD58 - READ_OCR (R3)
	R[0] = Commnd(CMD58, 0x00000000, 0xFF);
    if(R[0] == R1_READY_STATE)
	{
		R[1] = xfer_spi(0xFF);
		R[2] = xfer_spi(0xFF);
		R[3] = xfer_spi(0xFF);
		R[4] = xfer_spi(0xFF);
								sdhc = ((R[1]& 0xC0) == 0xC0) ? 1 : 0;
	}
	else
	{
		output_high(CS);
		return 1; 				//error
	}
  }// versionSD == 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  setup_spi(SPI_MASTER|SPI_SS_DISABLED|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_DIV_1);

  output_high(CS);   									// tarjeta deshabilitada
  return 0;
}//fin sd_init
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_write_block(unsigned int32 address, unsigned char* ptr){
   unsigned int16 iW;

   if(sdhc==0){ address <<= 9; }    //memoria SC -> bytes 		memoria HC -> sectores
   output_low(CS);

		iW = Commnd(CMD24, address, 0xFF);
    if( iW == R1_READY_STATE){
             								              		xfer_spi(0xFF);
                              									xfer_spi(DATA_START_BLOCK);   // Data start token
                           			for(iW = 0; iW < 512; iW++) xfer_spi(ptr[iW]);
                           										xfer_spi(0xFF);   // Dummy bytes
                          										xfer_spi(0xFF);   // en lugar del CRC16

                        do{       iW = xfer_spi(0xFF);
                        }while( (iW & DATA_RES_MASK) != DATA_RES_ACCEPTED);

                        do{       iW = xfer_spi(0xFF); delay_us(100);
                        }while(   iW == 0 );

						xfer_spi(0xFF);
						xfer_spi(0xFF);

   						output_high(CS);   return 0; //correcto
						}
   						output_high(CS);   return 1; //error
}//fin sd_write_block
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_read_block(unsigned int32 address, unsigned char* ptr){
   unsigned int16 jR;
   unsigned int8  iR;

    if(sdhc==0){ address <<= 9; }    //memoria SC -> bytes 		memoria HC -> sectores
    output_low(CS);

       iR  = Commnd(CMD17, address, 0xFF);
    if(iR == R1_READY_STATE){
								do{     iR  = xfer_spi(0xFF);
								}while( iR != DATA_START_BLOCK);

								if(iR == DATA_START_BLOCK){
								for(jR = 0; jR < 512; jR++)
								         dt[jR] = xfer_spi(0xFF);
								xfer_spi(0xFF);
								xfer_spi(0xFF);
								xfer_spi(0xFF);
  								}
   							output_high(CS);      return 0;	//correcto
							}
							output_high(CS);      return 1;	//error
}//fin sd_read_block
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_estructura()
{
	unsigned int32 sctrs;
	int16 RsrvdCnt;

	sd_read_block(0, dt);	//Lee el sector fisico 0 para obtener el LBA0
	if(dt[510]==0x55)
	if(dt[511]==0xAA){ 		// Efectivamente parece que estoy en sct 0
							// Tabla de particiones
								LBA0 = make32(dt[457],dt[456],dt[455],dt[454]);
								if(dt[0]==0xEB) // MBR
 								LBA0 = 0;	//Sector Logico 0
						  }//55AA
	else return 1; // No ha encontrado el LBA0 -> Hay que formatear la tarjeta

	sd_read_block(LBA0, dt);
		sect_x_cluster = (unsigned int32)dt[13] & 0x00FF;
		RsrvdCnt   	= make16(dt[15],dt[14]);
		RootEntCnt 	= make16(dt[18],dt[17]);
		sect_x_fat 	= make16(dt[23],dt[22]);
		MxSize		= make32(dt[35],dt[34],dt[33],dt[32]); // Medido en numero de sectores (512 bytes)


	/* LBA0 */		  sctrs  = LBA0;		// Inicio de cuenta: Direccion del Logic Block cero
				  	  sctrs += (unsigned int32) RsrvdCnt & 0xFFFF;

    sect_fat_1  	= sctrs;               	// A partir de ah�: FAT 1
				  	  sctrs += (unsigned int32) sect_x_fat & 0xFFFF;

	sect_fat_2  	= sctrs;				// A partir de ah�: FAT 2 (copia de seguridad)
				  	  sctrs += (unsigned int32) sect_x_fat & 0xFFFF;

	sect_entry  	= sctrs ;   			// A partir de ah� empieza el ra�z (la lista de ficheros)
					  sctrs += (unsigned int32)(RootEntCnt>>4)&0xFFFF;

	sect_ini_datos 	= sctrs ;				// A partir de ah� empieza el contenido de los ficheros

	cluster=cluster_ini=0x0005;

	sector=sector_ini=sect_ini_datos+(unsigned int32)((cluster_ini-2)*sect_x_cluster);

}//fin sd_estructura

// struct SDstruct SD_get_fields()
// {
//   struct SDstruct sdfields;
//
//   sdfields.MxSize=MxSize;
//   sdfields.sect_x_cluster=sect_x_cluster;
//   sdfields.RootEntCnt=RootEntCnt;
//   sdfields.sect_ini_datos=sect_ini_datos;
//   sdfields.sect_fat_1=sect_fat1;
//   sdfields.sect_fat_2=sect_fat2;
//   //por completar si es necesario
// }
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
