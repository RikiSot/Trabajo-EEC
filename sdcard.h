#bit  SPIRBF    = getenv("SFR:SPI1STAT").0
#bit  SPITBF    = getenv("SFR:SPI1STAT").1
#bit  SPIROV    = getenv("SFR:SPI1STAT").6
#byte SPI1BUF   = getenv("SFR:SPI1BUF")
#byte SPI1STAT  = getenv("SFR:SPI1STAT")
#byte SPI1CON1  = getenv("SFR:SPI1CON1")
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define   CMD0              		0x40      //go to idle				//Commands
#define   CMD1               		0x41      //initialization process
#define   CMD8               		0x48      //verify interface
#define   CMD17              		0x51      //read single block
#define   CMD24              		0x58      //write single block
#define   CMD55              		0x77      //escape for app specific command
#define   CMD58              		0x7a      //read OCR
#define   ACMD41             		0x69      //poll operation range
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define   R1_READY_STATE          	0x00      //sd ready				//Responses
#define   R1_IDLE_STATE           	0x01      //card in idle state
#define   R1_ILLEGAL_COMMAND      	0x04      //illegal command
#define   DATA_START_BLOCK       	0xFE      //start token for read or write
#define   DATA_RES_MASK          	0x1F      //mask for data response
#define   DATA_RES_ACCEPTED      	0x05      //write accepted
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sdhc;
unsigned int8 dt[512];            /// Buffer de escritura  en uSD
unsigned int32 LBA0 = 0;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
inline unsigned int8 xfer_spi(char envio){
   SPIROV = 0;
   SPI1BUF= envio;
   while( SPITBF);
   while(!SPIRBF);   
   return SPI1BUF;
}//fin xfer_spi
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 Commnd(char befF, int32 SD_Adress, char befH){
   unsigned int8 iC1;
   xfer_spi(0xFF);
   xfer_spi(befF);   
   xfer_spi(make8(SD_Adress, 3));
   xfer_spi(make8(SD_Adress, 2));
   xfer_spi(make8(SD_Adress, 1));
   xfer_spi(make8(SD_Adress, 0));
   xfer_spi(befH);
    do{iC1 = xfer_spi(0xFF);
   }while(iC1 == 0xFF);
   return iC1;
}//fin Commnd
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_read_block(unsigned int32 address, unsigned char* ptr);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_init(){
 unsigned  int8 R[17]={0}, versionSD= 1, crc;
 unsigned int16 iI;
 unsigned int32 arg=0;
 
 setup_spi(SPI_MASTER|SPI_SS_DISABLED|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_DIV_16);

 memset(dt,0,512);
 // CMD0 - GO_IDLE_STATE  (R1)• • Card Reset  • • • • • • • • • • • • • • • • • • • • • • • 
	do{	output_high(CS);   	// tarjeta deshabilitada  
		for (iI = 0; iI < 10; iI++)  	xfer_spi(0xFF);
		output_low(CS); 	// tarjeta habilitada
		R[0] = Commnd( CMD0 , 0x00000000 , 0x95); //go to idle
	}while( R[0] != R1_IDLE_STATE);
  // CMD8 - SEND_IF_COND (R7)  • Send Interface Condition Command• • • • • • • • • • • • • • 
  // Argument 00 - 00 - 01 (Voltage supplied = 2.7-3.6V) - AA (Check pattern)
  // Response: illegal command -> Version 1
  // Response: echo-back 	   -> Version 2
	R[0] = Commnd(CMD8, 0x000001AA, 0x87);
    if(R[0]& R1_ILLEGAL_COMMAND){	  			versionSD = 1;
   			}else{	R[1] = xfer_spi(0xFF);
					R[2] = xfer_spi(0xFF);
					R[3] = xfer_spi(0xFF);
					R[4] = xfer_spi(0xFF);
				    if(R[4]!=0xAA){	output_high(CS); return 1; }		//error   		     				  
      											versionSD = 2;
   			}

  // ACMD41 - SD_SEND_OP_ COND (R1) • • Initialization Command   • • • • • • • • • • • • • • 
  // If host supports SDHC or SDXC: HCS (Host Capacity Support) is set to 1 (arg= 0x40000000)
  // If host only support SDSC:     HCS (Host Capacity Support) is set to 0 (arg= 0)
   arg = (versionSD == 2) ? 0x40000000 : 0;
   crc = (versionSD == 2) ? 0x77 : 0xFF;

   
   do{            Commnd(CMD55,    0, 0x65);	// El proceso puede llegar a durar hasta 1 segundo
           R[0] = Commnd(ACMD41, arg, crc);		// Application-Specific Command – APP_CMD (CMD55)
		   delay_ms(4);
   }while(R[0] != R1_READY_STATE);

  if(versionSD == 2){

  // • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • 
  // CMD58 - READ_OCR (R3) • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • 
  // Bit 30 - Card Capacity Status bit: 	0 indicates that the card is SDSC. 
  //										1 indicates that the card is SDHC or SDXC
	R[0] = Commnd(CMD58, 0x00000000, 0xFF);
    if(R[0] == R1_READY_STATE){	R[1] = xfer_spi(0xFF);
								R[2] = xfer_spi(0xFF);
								R[3] = xfer_spi(0xFF);
								R[4] = xfer_spi(0xFF);	
								sdhc = ((R[1]& 0xC0) == 0xC0) ? 1 : 0;
   			}else{				output_high(CS); return 1; }		//error
  // • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • 

  }// versionSD == 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  setup_spi(SPI_MASTER|SPI_SS_DISABLED|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_DIV_1);

  // • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • 
	sd_read_block(0, dt);	//Lee el sector fisico 0 para obtener el LBA0
	if(dt[510]==0x55)
	if(dt[511]==0xAA){ 		// Efectivamente parece que estoy en sct 0
							// Tabla de particiones	
								LBA0 = make32(dt[457],dt[456],dt[455],dt[454]);         
								if(dt[0]==0xEB) // MBR
 								LBA0 = 0;
						  }//55AA
  // • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • • 

  output_high(CS);   									// tarjeta deshabilitada
  return 0;
}//char sd_init()- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 sd_write_block(unsigned int32 address, unsigned char* ptr){
   unsigned int16 iW;

   if(sdhc==0){ address <<= 9; }    //memoria SC -> bytes 		memoria HC -> sectores 
   output_low(CS);   

		iW = Commnd(CMD24, address, 0xFF);	
    if( iW == R1_READY_STATE){
             								              		xfer_spi(0xFF);
                              									xfer_spi(DATA_START_BLOCK);   // Data start token
                           			for(iW = 0; iW < 512; iW++)  xfer_spi(ptr[iW]);
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
								         ptr[jR] = xfer_spi(0xFF);
								xfer_spi(0xFF);
								xfer_spi(0xFF);
								xfer_spi(0xFF);
  								}

   							output_high(CS);      return 0;
							}			
							output_high(CS);      return 1;
}//fin sd_read_block
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
