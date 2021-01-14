#include "SD_Card.h"
#include "common.h"

extern unsigned int8 hour ;
extern unsigned int8 minute ;
extern unsigned int8 second ;

char cadena[32];
int jj;


//Inicialización de la SD
void sd_init_global(){
  set_tris_b(0b1111111110011111);
  set_tris_c(0b1111111111011111);
  delay_ms(30);

  sd_init();	// Inicializa micro SD

  initFAT();	// Carga los parametros del sistema de ficheros
  inicializa_fichero();
}

//Escritura en el fichero LATIDOS
void escritura_sd(){

  memset(dt,0,512);
  sprintf(cadena,"%%03d\n",PPM); //Escribe en el archivo LATIDOS
  output_toggle(H1);

  for(jj=0;jj<3;jj++)
  {
    dt[jj+(3)] = cadena[jj];
  }
//	escribe_datos_en_fichero();
    escribe_datos_en_fichero();

}
