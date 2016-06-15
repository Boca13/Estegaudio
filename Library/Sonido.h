#include <stdlib.h>
#include <irrKlang.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#pragma once
#ifdef _WIN32 //Seleccionar compilación
#define DLL __declspec(dllexport)
#else
#define DLL 
#endif

namespace Sonido
{
	//Estructura que usamos para referenciarnos al buffer de audio despues de grabar el mensaje.
	//Esta formada por un puntero que apunta a la direccion de comienzo del buffer de audio y por un int que contiene el tamaño
	//del buffer de audio
	typedef struct
	{
		void * direccion;
		int tam;
	}sonido;

	//Sobrecarga de metodo: Si no le pasamos ningun parámetro este no guarda la grabación en un archivo externo.
	//Simplemente rellena el buffer de audio con la grabación y ya.
	//DLL sonido Sonido::grabarMensaje();

	//Sobrecarga de metodo: Si le pasamos el parámetro nombreArchivo este guardara la grabación en un archivo externo.
	//El nombre del archivo sera el de la cadena nombreArchivo.
	DLL int grabarMensaje(char* nombreArchivo, char grabar);

	//Metodo que usamos para liberar la memoria cuando no nos haga mas falta el buffer de audio ni nada. Libera la memoria 
	//del buffer y manda el puntero a apuntar a NULL (mirar estructura sonido arriba)
	DLL void eliminarSonido(sonido s);
	
	//Carga un sonido en el buffer para poder trabajar con el. El archivo sera especificado por la cadena nombreArchivo
	DLL sonido cargarSonidoArchivoExterno(char * nombreArchivo);

	
	//Funcion de la biblioteca irrklang usada por los otros metodos para crear el archivo .wav que guarda la grabacion 
	//(en el caso de que usemos grabarmensaje() con un parametro). La llamada la realiza grabarMensaje().
	DLL void  writeWaveFile(const char* filename, irrklang::SAudioStreamFormat format, void* data);

};

