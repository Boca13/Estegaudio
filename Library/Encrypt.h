//#include <mcrypt.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#pragma once
#ifdef _WIN32 //Seleccionar compilación
#define DLL __declspec(dllexport)
#else
#define DLL 
#endif

public class Encrypt
{
public:
	Encrypt();
	~Encrypt();
	// Encripta los datos en memoria y devuelve el puntero al resultado y el tamaño de la información (por referencia).
	DLL void* encriptar(void* memoria, int tam, int* tamres);
	DLL void encriptar(char* origen, char* destino);
	DLL void* desencriptar(void* memoria, int tam, int* tamres);
	DLL void desencriptar(char* origen, char* destino);
};

