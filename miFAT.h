// Definidos en sdcard.h - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern unsigned int8 dt[512];
extern unsigned int32 LBA0;
extern unsigned int8 sd_init();
extern unsigned int8 sd_write_block(unsigned int32 address, unsigned char* ptr);
extern unsigned int8 sd_read_block( unsigned int32 address, unsigned char* ptr);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define MAX_ENTRIES_SECTOR_FAT16        256
#define ENDFILE16                    0xFFFF
#define MAX_ENTRIES_SECTOR_FAT32        128
#define ENDFILE32                0x0FFFFFFF
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct{
	union{
		unsigned int8 raw[32];
		struct fields{
			char name[8];
			char extension[3];
			unsigned int8 attrib;			///s�lo lectura, oculto, de sistema, directorio...
			//unsigned int8 reserved[10];
			  unsigned int8  reserved;
	 		  unsigned int8  createTimeMs;
			  unsigned int16 create_hour;
			  unsigned int16 create_date;
			  unsigned int16 last_access;
			  unsigned int16 eaIndex;
			  unsigned int16 modif_hour;
			  unsigned int16 modif_date;
			  unsigned int16 first_cluster;	///primer cluster (direcciona tabla FAT y sector para escribir contenido)
			  unsigned int32 size;			///tama�o en bytes
		};
	};
} fileEntry;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef struct{
	unsigned int32 currentCluster;
	unsigned int32 currentSector;
	unsigned int16 posInSector;      //posici�n actual al escribir en el sector
	unsigned int32 sectorAtFAT;      //sector de la 1� FAT en el que tenemos el actual puntero de cierre de fichero

	unsigned int32 sectorEntryFile; //direcci�n del sector de entradas de fichero
	unsigned int8  EntryFile[512];  //sector donde est� la entrada (root) del fichero (para actualizar tama�o)
	unsigned int16 offsetEntry;     //offset en el sector de entradas del fichero
	unsigned int32 size;            //tama�o del fichero
}_file;

_file file;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct _diskinforec{
   unsigned int8  jmpBoot[3];   // bytes  0.. 2: Salto a la rutina de arranque
   unsigned int8  OEMName[8];   // bytes  3..10: Es una cadena "MSDOS5.0"
   unsigned int16 BytsPerSec;   // bytes 11..12: Numero de bytes por sector (00 02 = 0x0200 = 512 bytes)
   unsigned int8  SectPerClus;  // byte  13:    Numero de sectores contenidos en 1 cluster Nota:el valor (BytsPerSec*SectPerClus) <= 32 KBytes
   unsigned int16 RsrvdCnt;     // bytes 14..15: Sectores reservados del volumen comenzando por el sector de volumen (debe ser >0) FAT16=1 FAT32=32 habitualmente
   unsigned int8  NumFATs;      // byte  16:    Numero de tablas FAT en el volumen. Habitualmente es 2
   unsigned int16 RootEntCnt;   // bytes 17..18: Numero de entradas de 32 bytes al directorio raiz. Habitualmente FAT16= 512 y FAT32= 0
   unsigned int16 TotSect16;    // bytes 19..20: Numero de sectores del volumen (si vale 0 es porque es > 0x10000=65536 y entonces TotSec32 >0)
   unsigned int8  Media;        // byte  21:    Hab. 0xF8 (medio fijo) Son legales tambien: F9, FA, FB, FC, FD, FE, FF, F0
   unsigned int16 FATsz16;      // bytes 22..23: Sectores ocupados por una FAT16 (si es FAT32 -> es 0)
   unsigned int16 SectPerTrack; // bytes 24..25: Sectores por track
   unsigned int16 NumHeads;     // bytes 26..27: Numero de cabezas (el anterior y este tiene sentido en diskettes, cintas, etc.)
   unsigned int32 HiddSec;      // bytes 28..31: Sectores ocultos que preceden a la particion (irrelevante en uSD)
   unsigned int32 TotSect32;    // bytes 32..35: Numero de sectores del volumen (TotSect32*BytsPerSec) es la capacidad de la tarjeta
   unsigned int32 FATsz32;      // bytes 36..39: Numero de sectores ocupados por una FAT
   unsigned int16 ExtFlags;     // bytes 40..41: Flags usados por el sistema (no se usan y se deja a 0)
   unsigned int16 FSVer;        // bytes 42..43: Version del sistema de archivos (empleamos la 0.0)
   unsigned int32 RootClus;     // bytes 44..47: Indica el numero del 1er cluster del directorio raiz.
   unsigned int16 FSInfo;       // bytes 48..49: Indica el numero del sector de la estructura FSInfo (suele ser 1)
   unsigned int16 BkBootSec;    // bytes 50..51: Indica el numero del sector donde esta la copia de seguridad del sector de arranque
   unsigned int8  RsrvdSys[12]; // bytes 52..63: Bytes reservados para futuros usos del sistema
   unsigned int8  DrvNum;       // byte  64:     Numero de la unidad (para MSDOS pero irrelevante en uSD).
   unsigned int8  Reserved1;    // byte  65:     Windows NT para formatear un volumen pone este byte a 0
   unsigned int8  BootSig;      // byte  66:     Firma digital= 0x29 (si no lo es -> volumen no reconocido por Windows)
   unsigned int32 VolId;        // bytes 67..70: Numero de volumen (generado por el reloj del sistema).
   unsigned int8  VolLab[11];   // bytes 71..81: Es una cadena etiqueta del volumen suele ser "NO NAME     ". Nosotros podemos poner "NUUBO_SD    "
   unsigned int8  FilSysType[8];// bytes 82..89: Es una cadena con el nombre de sistema de archivos (p.ej. "FAT32   "
   unsigned int8  spam[420];    // bytes 90..509:Zona de codigo (no lo empleamos y esta a 0)
   unsigned int16 firma;        // byte 510..511:Debe de vale 55 AA
} diskinforec;

diskinforec DiskInfo;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned  int8 FATType; // FAT16 o FAT32
//variables clave: inicio de la FAT1 y la FAT2 y de las entradas de ficheros
unsigned  int8 sect_x_cluster;
unsigned int16 sect_fat_1;
unsigned int16 sect_fat_2;
unsigned int32 sect_ini_datos;
unsigned int32 sect_x_fat;
unsigned int32 sect_entry;
unsigned int16 sect_reserv;

//unsigned int8 dFil[32];
unsigned int32 sector_fat32[MAX_ENTRIES_SECTOR_FAT32]; // 128 x 4 = 512
unsigned int16 sector_fat16[MAX_ENTRIES_SECTOR_FAT16]; // 256 x 2 = 512

//atributos
#define __FILE		0x20
#define _DIRECTORY	0x10
#define _VOLUMEN	0x08
#define _SYSTEM		0x04
#define _HIDDEN		0x02
#define READONLY	0x01

///variables globales
unsigned int16 year = 2020;
unsigned int8 yearLo=   20;
unsigned int8 month =   10;
unsigned int8 day   =	15;
unsigned int8 hour	=   12;
unsigned int8 minute=   34;
unsigned int8 second=   56;

///funciones
#define getHour() (int16)((int16)hour << 11) + (int16)((int16)minute << 5) + (int16) (second >> 1)		//5 bits: horas, 6 bits: minutos, 5 bits: segundos/2
#define getDate() (((int16)year - 1980) << 9 ) + ((int16)month << 5) + (int16)day	//7 bits: a�o (desde 1980), 4 bits: mes, 5 bits: dia


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void initFAT(){
   unsigned int32 startClusterOfRootDirectory = 2;
   unsigned int32 sctrs;
   unsigned  int8 *ptrSct;
   diskinforec *ptrDiskInfo;

	sd_read_block(LBA0, dt);
    						ptrDiskInfo=&DiskInfo;
    						ptrSct= &dt[0];
   	memcpy((*ptrDiskInfo).jmpBoot, (ptrSct +  0), 3); 			//DiskInfo.jmpBoot[3]	// bytes  0.. 2: Salto a la rutina de arranque
   	memcpy((*ptrDiskInfo).OEMName, (ptrSct +  3), 8); 			//DiskInfo.OEMName[8]	// bytes  3..10: Es una cadena, habitualmente "MSDOS5.0"
   	DiskInfo.BytsPerSec = make16(dt[12],dt[11]);   				// bytes 11..12: Numero de bytes por sector (00 02 = 0x0200 = 512 bytes)
	DiskInfo.SectPerClus = dt[13];								// byte  13:	 Numero de sectores contenidos en un cluster Nota:el valor (BytsPerSec*SectPerClus) <= 32 KBytes
	DiskInfo.RsrvdCnt = make16(dt[15],dt[14]);					// bytes 14..15: Sectores reservados del volumen comenzando por el sector de volumen (debe ser >0) FAT16=1 FAT32=32 habitualmente
   	DiskInfo.NumFATs = dt[16];									// byte  16:	 Numero de tablas FAT en el volumen. Habitualmente es 2
	DiskInfo.RootEntCnt = make16(dt[18],dt[17]);				// bytes 17..18: Numero de entradas de 32 bytes al directorio raiz. Habitualmente FAT16= 512 y FAT32= 0
   	DiskInfo.TotSect16 = make16(dt[20],dt[19]);					// bytes 19..20: Numero de sectores del volumen (si vale 0 es porque es > 0x10000=65536 y entonces TotSec32 >0)
	DiskInfo.Media = dt[21];									// byte  21:	 Hab. 0xF8 (medio fijo) Son legales tambien: F9, FA, FB, FC, FD, FE, FF, F0
	DiskInfo.FATsz16 = make16(dt[23],dt[22]);					// bytes 22..23: Sectores ocupados por una FAT16 (si es FAT32 -> es 0)
	DiskInfo.SectPerTrack = make16(dt[25],dt[24]); 				// bytes 24..25: Sectores por track
   	DiskInfo.NumHeads = make16(dt[27],dt[26]);					// bytes 26..27: Numero de cabezas (el anterior y este tiene sentido en diskettes, cintas, etc.)
	DiskInfo.HiddSec = make32(dt[31],dt[30],dt[29],dt[28]);		// bytes 28..31: Sectores ocultos que preceden a la particion
	DiskInfo.TotSect32=make32(dt[35],dt[34],dt[33],dt[32]);		// bytes 32..35: Numero de sectores del volumen (TotSect32*BytsPerSec) es la capacidad de la tarjeta
	DiskInfo.FATsz32 = make32(dt[39],dt[38],dt[37],dt[36]);		// bytes 36..39: Numero de sectores ocupados por una FAT
	DiskInfo.ExtFlags = make16(dt[41],dt[40]); 					// bytes 40..41: Flags usados por el sistema (no se usan y se deja a 0)
	DiskInfo.FSVer = make16(dt[43],dt[42]); 					// bytes 42..43: Version del sistema de archivos (empleamos la 0.0)
	DiskInfo.RootClus= make32(dt[47],dt[46],dt[45],dt[44]);		// bytes 44..47: Indica el numero del 1er cluster del directorio raiz.
	DiskInfo.FSInfo = make16(dt[49],dt[48]); 					// bytes 48..49: Indica el numero del sector de la estructura FSInfo (suele ser 1)
	DiskInfo.BkBootSec = make16(dt[51],dt[50]); 				// bytes 50..51: Indica el numero del sector donde esta la copia de seguridad del sector de arranque
	//  Estructura en el caso de FAT32 �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �
	memcpy((*ptrDiskInfo).RsrvdSys, (ptrSct +  52), 12);		//DiskInfo.RsrvdSys[12] 	// bytes 52..63: Bytes reservados para futuros usos del sistema
	DiskInfo.DrvNum = dt[64];								 	// byte  64: 	 Numero de la unidad
	DiskInfo.Reserved1 = dt[65];								// byte  65: 	 Windows NT para formatear un volumen pone este byte a 0
	DiskInfo.BootSig = dt[66]; 									// byte  66: 	 Firma digital= 0x29 (si no lo es -> volumen no reconocido por Windows)
	DiskInfo.VolId = make32(dt[70],dt[69],dt[68],dt[67]); 		// bytes 67..70: Numero de volumen (generado por el reloj del sistema).
	memcpy((*ptrDiskInfo).VolLab, (ptrSct +  71), 11);			//DiskInfo.VolLab[11]	// bytes 71..81: Es una cadena etiqueta del volumen suele ser "NO NAME    "
	memcpy((*ptrDiskInfo).FilSysType, (ptrSct +  82), 8);		//DiskInfo.FilSysType[8] 	// bytes 82..89: Es una cadena con el nombre de sistema de archivos (p.ej. "FAT32   "
	memcpy((*ptrDiskInfo).spam, (ptrSct +  90),420);			//DiskInfo.spam[420]		// bytes 90..509:Zona de codigo (en uSD no se emplea y suele estar a 0)
	DiskInfo.firma = make16(dt[510],dt[511]);								// byte 510..511:Debe de vale 55 AA

   		FATType = (DiskInfo.FilSysType[3]-'0')*10+ DiskInfo.FilSysType[4]-'0'; // Vale de tomar FAT'3''2'

   if(FATType == 32){
						sect_x_fat  = DiskInfo.FATsz32;		// Sectores reservados para la FAT
						startClusterOfRootDirectory = DiskInfo.RootClus;
 	}else{
						memcpy((*ptrDiskInfo).FilSysType, (ptrSct +  54), 8);	//DiskInfo.FilSysType[8] 	// bytes 54..61: Es una cadena con el nombre de sistema de archivos (p.ej. "FAT16   "
						FATType = (DiskInfo.FilSysType[3]-'0')*10+ DiskInfo.FilSysType[4]-'0'; // Vale de tomar FAT'1''6'
	}

	if(FATType == 16){
						DiskInfo.VolId = make32(dt[42],dt[41],dt[40],dt[39]);
						memcpy((*ptrDiskInfo).VolLab, (ptrSct +  43), 11);
						memcpy((*ptrDiskInfo).FilSysType, (ptrSct +  54), 8);

						sect_x_fat  = DiskInfo.FATsz16;		// Sectores reservados para la FAT
						startClusterOfRootDirectory = DiskInfo.RootEntCnt;
 					}

  	sect_x_cluster = DiskInfo.SectPerClus;					// Cada cluster tiene este numero de sectores

    			      sctrs  = LBA0;
				  	  sctrs += DiskInfo.RsrvdCnt;
	sect_reserv 	= sctrs;								// sectores reservados por el sistema

    sect_fat_1  	= sctrs;               					// A partir de ah�: FAT 1
				  	  sctrs += sect_x_fat;
	sect_fat_2  	= sctrs;;								// A partir de ah�: FAT 2 (copia de seguridad)
				  	  sctrs += sect_x_fat;
	sect_entry  	= sctrs ;      							// A partir de ah� empieza el ra�z (la lista de ficheros)
					  sctrs += sect_x_cluster;
	sect_ini_datos 	= sctrs ;								// A partir de ahi los datos del fichero

    if(FATType == 16)										// RootEntCnt: Numero de entradas de 32 bytes al
    sect_ini_datos = sect_entry+(DiskInfo.RootEntCnt>>4);	// directorio raiz. Habitualmente FAT16= 512 y FAT32= 0

}//fin initFAT()
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void cargaConfig(unsigned int32 j);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 encontrar_fichero(){
   unsigned int8 FicheroEncontrado = 0;
   unsigned int32 i;
   unsigned int16 j,k=0;
   unsigned int32 SctStart = sect_entry;			//inicio Root
   unsigned int32 SctEnd   = sect_ini_datos;		//fin    Root
   unsigned int32 ClstIni  = ((FATType==16)?1:2);

   unsigned int32 ClstNxt;
   unsigned int32 sctrClstr;
   unsigned int32 posSctr;
   unsigned int32 clstrSig;

   for(i = SctStart; i < SctEnd; i++){
	      sd_read_block(i, dt);
		 for(j = 0; j < 512; j += 32){
										k=0;
										if(	(dt[j + 0] == 'C')&&
											(dt[j + 1] == 'O')&&
											(dt[j + 2] == 'N')&&
											(dt[j + 3] == 'F')&&
											(dt[j + 4] == 'I')&&
											(dt[j + 5] == 'G')&&
											(dt[j + 6] == ' ')&&
											(dt[j + 7] == ' ')&&
											(dt[j + 8] == 'T')&&
											(dt[j + 9] == 'X')&&
											(dt[j +10] == 'T')){
																 cargaConfig(j);
																 FicheroEncontrado = 1;
																 return FicheroEncontrado;
																}//fin if(CNF)

		}//j
   }//i
   return FicheroEncontrado;
} // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void cargaConfig(unsigned int32 j){
   unsigned int32 direccion;
   unsigned int32 _sector;

	if(FATType==16){
	   	direccion = make32(0,0,dt[j+27],dt[j+26]);
		_sector = ((unsigned int32)direccion - 2) * sect_x_cluster + sect_ini_datos;
	}
	if(FATType==32){
	   direccion = make32(dt[j+21],dt[j+20],dt[j+27],dt[j+26]); //en FAT32 se usa EAIndex + first cluster
	   _sector = ((unsigned int32)direccion - 3) * sect_x_cluster + sect_ini_datos;
   }
	sd_read_block(_sector, dt);
   // Ahora en el vector dt tengo los datos del fichero y puedo emplearlo
   // Por ejemplo: el texto que indica la hora y minutos del ensayo lo obtendria como:
   // hora   = (dt[140]-'0')*10 + dt[141]-'0' ;
   // minutos= (dt[143]-'0')*10 + dt[144]-'0' ;
   // ... Y con esos datos haria algo util

} // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int32 encontrarCluster(unsigned int32 clusterStart);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int8 inicializa_fichero(){
   unsigned int8 FicheroIncializado = 0;
   unsigned int32 i;
   unsigned int16 j;
   unsigned int32 SctStart = sect_entry;			//inicio Root
   unsigned int32 SctEnd   = sect_ini_datos;		//fin    Root
   unsigned int32 ClstIni  = ((FATType==16)?0:1);

   unsigned int32 ClstNxt;
   unsigned int32 sctrClstr;
   unsigned int32 posSctr;
   unsigned int32 clstrSig;

   unsigned int32 startCluster;      //cluster donde empezar�an los datos del fichero
   fileEntry newFile;

   for(i = SctStart; i < SctEnd; i++){
	      sd_read_block(i, dt);
		  for(j = 0; j < 512; j += 32){
			if((dt[j] == 0x00) || dt[j] == 0xFF){
					   newFile.fields.name[0] = 		'L';
					   newFile.fields.name[1] = 		'A';
					   newFile.fields.name[2] = 		'T';
					   newFile.fields.name[3] = 		'I';
					   newFile.fields.name[4] = 		'D';
					   newFile.fields.name[5] = 		'O';
					   newFile.fields.name[6] = 		'S';
					   newFile.fields.name[7] = 		'_';
					   newFile.fields.extension[0] = 	'T';
					   newFile.fields.extension[1] = 	'X';
					   newFile.fields.extension[2] = 	'T';
					   newFile.fields.reserved = 0;
					   newFile.fields.createTimeMs = 0;
					   newFile.fields.create_hour = getHour();
					   newFile.fields.create_date = getDate();
					   newFile.fields.last_access = newFile.fields.create_date;
					   newFile.fields.eaIndex = 0;
					   newFile.fields.attrib = __FILE ;   //archivo normal + s�lo lectura  | READONLY
					   newFile.fields.modif_hour = newFile.fields.create_hour;
					   newFile.fields.modif_date = newFile.fields.create_date;
					   newFile.fields.first_cluster = 0;
					   newFile.fields.size = 0;

					   file.offsetEntry = j;
					   file.sectorEntryFile = i;
					   sd_read_block(file.sectorEntryFile, file.EntryFile);

					   for(i = 0; i < 32; j++, i++)  dt[j] = newFile.raw[i];
					   for(i = 0; i < 512; i++)      file.EntryFile[i] = dt[i];
					   file.posInSector    = 0;   															//empezamos en un sector y cluster nuevos
					   if(FATType==16){
					   startCluster = encontrarCluster(2);         											//busco cluster donde empezar�a a almacenarse los datos
					   file.currentCluster = startCluster;   												//que ser� el cluster en el que escribiremos
					   file.currentSector  = (file.currentCluster -  2) * sect_x_cluster + sect_ini_datos;  //sector en el que escribiremos (es el primero del cluster)
					   file.sectorAtFAT    = (file.currentCluster >> 8) + sect_reserv;						//sector en la FAT donde marcar el cluster usado
					   }
					   if(FATType==32){
					   startCluster = encontrarCluster(3);         											//busco cluster donde empezar�a a almacenarse los datos
					   file.currentCluster = startCluster;   												//que ser� el cluster en el que escribiremos
					   file.currentSector  = (file.currentCluster -  3) * sect_x_cluster + sect_ini_datos;  //sector en el que escribiremos (es el primero del cluster)
					   file.sectorAtFAT    = (file.currentCluster >> 7) + sect_reserv;						//sector en la FAT donde marcar el cluster usado
					   }
					   file.EntryFile[file.offsetEntry + 27] = make8(file.currentCluster, 1); 					//1er cluster del archivo
					   file.EntryFile[file.offsetEntry + 26] = make8(file.currentCluster, 0);
					   file.EntryFile[file.offsetEntry + 21] = make8(file.currentCluster, 3);
					   file.EntryFile[file.offsetEntry + 20] = make8(file.currentCluster, 2);
					   file.size = 0;																			//Inicialmente el tama�o es cero

					   if(FATType==16){
						sd_write_block(file.sectorEntryFile, file.EntryFile);
						sd_read_block (file.sectorAtFAT, sector_fat16);
						sector_fat16[file.currentCluster % MAX_ENTRIES_SECTOR_FAT16] = ENDFILE16;				//marco el cluster como usado
						sd_write_block( file.sectorAtFAT, sector_fat16);
						sd_write_block((file.sectorAtFAT+sect_x_fat),sector_fat16);   //FAT2					//�Ojo! la cosa puede ser mas complicada...
						}
					   if(FATType==32){
						sd_write_block(file.sectorEntryFile, file.EntryFile);
						sd_read_block (file.sectorAtFAT,sector_fat32);
						sector_fat32[file.currentCluster % MAX_ENTRIES_SECTOR_FAT32] = ENDFILE32;				//marco el cluster como usado
						sd_write_block( file.sectorAtFAT, sector_fat32);
						sd_write_block((file.sectorAtFAT+sect_x_fat),sector_fat32);   //FAT2					//�Ojo! la cosa puede ser mas complicada...
						}
					   FicheroIncializado = 1;
					   return FicheroIncializado;
					   }

		}//j
   }//i
   return FicheroIncializado;
} // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
unsigned int32 encontrarCluster(unsigned int32 clusterStart){
   unsigned int32 cluster;
   unsigned int32 _sector;   //recorrer sectores
   unsigned int32 bytes;     //recorrer bytes en cada sector

   cluster =  clusterStart;
   if(FATType==16)
   _sector = (clusterStart >> 2) + sect_fat_1;	// Empiezo a buscar desde donde estoy en adelante
   if(FATType==32)
   _sector = (clusterStart >> 3) + sect_fat_1;	// Empiezo a buscar desde donde estoy en adelante
   bytes = (clusterStart % ((FATType==16) ? MAX_ENTRIES_SECTOR_FAT16 : MAX_ENTRIES_SECTOR_FAT32));

   if(FATType==32){
	   for(;_sector < sect_fat_2; _sector++){      						//recorremos los sectores de la FAT
		  sd_read_block(_sector,sector_fat32); 							//leemos el sector
    					      for(; bytes < MAX_ENTRIES_SECTOR_FAT32; bytes++){ //recorremos el sector para buscar un puntero a cluster libre
					         if(sector_fat32[bytes]== 0){
					            if(cluster < 3)               continue;  	//si hubiera un error y el primer cluster de la fat apareciera como disponible, seguimos buscando
					            return cluster;
					         }//fin if
					         cluster++;
					      }//fin for de recorrer bytes dentro de un sector
	      bytes = 0;
	   }//fin for de recorrer sectores
   }else
    if(FATType==16){
	   for(;_sector < sect_fat_2; _sector++){      						//recorremos los sectores de la FAT
		  sd_read_block(_sector,sector_fat16); 							//leemos el sector

					      for(; bytes < MAX_ENTRIES_SECTOR_FAT16; bytes++){ 	//recorremos el sector para buscar un puntero a cluster libre
					         if(sector_fat16[bytes]== 0){
					            if(cluster < 2)               continue;		//si hubiera un error y el primer cluster de la fat apareciera como disponible, seguimos buscando
					            return cluster;
					         }//fin if
					         cluster++;
					      }//fin for de recorrer bytes dentro de un sector
	      bytes = 0;
	   }//fin for de recorrer sectores
	}
   return ((FATType==16)?ENDFILE16:ENDFILE32);
}//fin encontrarCluster
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
extern int cont;
void escribe_datos_en_fichero(){
   unsigned int32 relSector;
   unsigned int32 nextCluster;
   unsigned int32  oldCluster;
   unsigned int16 _hour, date;

	sd_write_block(file.currentSector, dt);


		if(cont>127)
		{
	 		file.currentSector++;	//actualizar las FATs
		}


		if(FATType==16)
	    relSector= file.currentSector - ((file.currentCluster -  2) * sect_x_cluster + sect_ini_datos);
		if(FATType==32)
	    relSector= file.currentSector - ((file.currentCluster -  3) * sect_x_cluster + sect_ini_datos);

	    if((relSector % sect_x_cluster) == 0){   											//�el siguiente sector est� en cluster nuevo?
	      oldCluster = 	file.currentCluster;
	      nextCluster = encontrarCluster(oldCluster);   									//siguiente cluster
	      //escribirCluster(oldCluster, nextCluster);  										//marcar actual cluster para que apunte al siguiente
	      file.currentCluster = nextCluster;
		  if(FATType==16)
	      file.currentSector = (nextCluster - 2) * sect_x_cluster + sect_ini_datos;
		  if(FATType==32)
	      file.currentSector = (nextCluster - 3) * sect_x_cluster + sect_ini_datos;
	   }

	   //actualizar el tama�o del fichero y fechas
	   file.size += 512;
	   file.EntryFile[file.offsetEntry + 28] = make8(file.size, 0);   //tama�o
	   file.EntryFile[file.offsetEntry + 29] = make8(file.size, 1);
	   file.EntryFile[file.offsetEntry + 30] = make8(file.size, 2);
	   file.EntryFile[file.offsetEntry + 31] = make8(file.size, 3);
	   //ActualizaReloj();
	   _hour = getHour();
	   date = getDate();
	   file.EntryFile[file.offsetEntry + 24] = make8(date, 0);       //fecha modif
	   file.EntryFile[file.offsetEntry + 25] = make8(date, 1);
	   file.EntryFile[file.offsetEntry + 22] = make8(_hour, 0);      //hora modif
	   file.EntryFile[file.offsetEntry + 23] = make8(_hour, 1);
	   file.EntryFile[file.offsetEntry + 18] = file.EntryFile[file.offsetEntry + 24];   //fecha �ltimo acceso
	   file.EntryFile[file.offsetEntry + 19] = file.EntryFile[file.offsetEntry + 25];

	   sd_write_block(file.sectorEntryFile, file.EntryFile);  	// Actualiza root
		 if(cont>127)
		 	{
 			//file.currentSector++;	//actualizar las FATs
 			memset(dt,0,512);
 			cont = 0;
 			}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void sd_init_global(){
  set_tris_b(0b1111111110011111);
  set_tris_c(0b1111111111011111);
  delay_ms(30);

  sd_init();	// Inicializa micro SD

  initFAT();	// Carga los parametros del sistema de ficheros
	inicializa_fichero();
}
