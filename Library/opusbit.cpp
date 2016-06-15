// reading a text file

#include "opusbit.h"




void codifica( char* audio, char *salida)
{
  //funcion que codifica el archivo de audio .wav a .opus
	char cadena[OPUS_TAM];
	sprintf(cadena," %s %s %s", OPUSENC ,audio, salida);
	int error = system(cadena);


}


void decodifica( char* wav, char * bitfile )
{
  //funcion que convierte .opus en bits 
	char cadena[OPUS_TAM];
	sprintf(cadena, " %s %s %s",OPUSDEC ,wav, bitfile);
	int error = system(cadena);

}





int main( int argc, char * argv[] )
{

 codifica( argv[1] , argv[2] );
 
 decodifica(argv[2] ,argv[3]);
	return 0;
}
