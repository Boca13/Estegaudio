#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#define OPUS_TAM 530

#ifdef _WIN32 //Seleccionar compilación
#define ENC "flac.exe -0"
#define DLL __declspec(dllexport)
#else
#define ENC "flac -0"
#define DLL
#endif

#ifdef _WIN32 //Seleccionar compilación
#define DEC "flac.exe -d"
#else
#define DEC "flac -d"
#endif


namespace Codec
{
	DLL void codifica(char* audioEntrada, char* flacSalida);
	DLL void decodifica(char* flacEntrada, char* wavSalida);
};