#include <stdlib.h>

#include "Codec.h"
#include "Red.h"
#include "Encrypt.h"
#include "Sonido.h"



#pragma once

#define MAX_CHATS 15
#define OFFSET 64

#ifdef _WIN32 //Seleccionar compilación
#define DLL __declspec(dllexport)
#else
#define DLL 
#endif

namespace Estegaudio
{
	DLL int introducir(char* archivo, char* sonido, char* destino);
	DLL int extraer(char* sonido, char* destino);
};

