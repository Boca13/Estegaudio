#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#define OPUS_TAM 530
//#define PARAM_OPUS1 "8000 2 80"

//#pragma once

#ifdef _WIN32 //Seleccionar compilación
#define OPUSENC"opusenc.exe"
#else
#define OPUSENC "opusenc"
#endif

#ifdef _WIN32 //Seleccionar compilación
#define OPUSEDEC"opusdec.exe"
#else
#define OPUSDEC "opusdec"
#endif




class Codec
{
public:
	Codec();
	~Codec();
	void * codifica(char* audio, char* vsalida);
	void * decodifica (char* wav,char* bitfile);
};
