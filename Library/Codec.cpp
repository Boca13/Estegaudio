#include "Codec.h"

void Codec::codifica(char* wavEntrada, char *flacSalida)
{
	//funcion que codifica el archivo de audio .wav a .opus
	char cadena[OPUS_TAM];
	sprintf(cadena, "%s %s -o %s -f", ENC, wavEntrada, flacSalida);
	int error = system(cadena);
}


void Codec::decodifica(char* flacEntrada, char * wavSalida)
{
	//funcion que convierte .opus en bits 
	char cadena[OPUS_TAM];
	sprintf(cadena, "%s %s -o %s -f", DEC, flacEntrada, wavSalida);
	int error = system(cadena);
}