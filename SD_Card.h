#ifndef INC_SD_CARD

#define INC_SD_CARD 2
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//Commands
#define   CMD0              		0x40      //go to idle
#define   CMD8               		0x48      //verify interface
#define   CMD17               		0x51      //read single block
#define   CMD24               		0x58      //write single block
#define   CMD55               		0x77      //escape for app specific command
#define   CMD58               		0x7a      //read OCR
#define   ACMD41               		0x69      //poll operation range
//Responses
#define   R1_READY_STATE          	0x00      //sd ready
#define   R1_IDLE_STATE           	0x01      //card in idle state
#define   R1_ILLEGAL_COMMAND      	0x04      //illegal command
#define   DATA_START_BLOCK       	0xFE      //start token for read or write
#define   DATA_RES_MASK          	0x1F      //mask for data response
#define   DATA_RES_ACCEPTED      	0x05      //write accepted
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Variables públicas
unsigned int8  sdhc;
unsigned int8  dt[512];            /// Buffer de escritura  en uSD
unsigned int32 LBA0;


struct SDstruct{
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
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
inline unsigned int8 xfer_spi(char);
unsigned int8 Commnd(char, int32, char);
unsigned int8 sd_init();
unsigned int8 sd_write_block(unsigned int32, unsigned char*);
unsigned int8 sd_read_block(unsigned int32, unsigned char*);

unsigned int8 sd_estructura();
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
