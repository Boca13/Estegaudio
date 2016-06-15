#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#pragma once

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define DLL __declspec(dllexport)
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif


#define BUFLEN 1490  //Max length of buffer
//#define PORT 8888   //The port on which to listen for incoming data
#define TAM_HASH 40

/*	FORMATO DE LA CABECERA
*
*		| "VOIP" | tipo de mensaje ('e'|'a') | hash usuario | hash mensaje (hora) | trozo | nº trozos | datos sonido |
*		|	4B	 |				1B			 |		40B		|			40B		  |	 2B	  |		2B	  |	hasta 1451 B |
*		|											49B												  |		1451B	 |
*/

#define CABECERA "VOIP"
#define TAM_CABECERA 89
//#define PUERTO 8888


namespace Red
{
	//Eliminar el socket
	DLL void dispose(unsigned int s);
	// Conectar con el servidor de presencia para registrar al usuario.
	//DLL static bool registrar(char* usuario);
	// Cerrar la sesión del usuario en el servidor.
	//DLL static void cerrarSesion(char* usuario);
	// Envía el mensaje identificado por el hash al usuario dado
	DLL char enviaMensaje(unsigned int s, char* ip, char* usuario, char *archivo, char *msj, unsigned short puerto);

	//http://stackoverflow.com/questions/12662891/c-passing-member-function-as-argument
	/*	Necesita que se le pase la función a la que llamará cuando reciba algún mensaje.
		Ésta recibe como parámetros el nombre de archivo donde se guarda el mensaje, su tamaño y el remitente*/
	DLL void recibeMensajes(unsigned int s, void(*funcion)(char *, int, char*));
	void recibeMensajes2(unsigned int s);

	// Primitiva que utilizará cada instancia de chat para configurar un socket por el que recibir mensajesvoi
	DLL unsigned int configurarSocket(char* ipaddr, unsigned short puerto);
	unsigned int configurarSocketEnvio();

	typedef char hash[TAM_HASH];
	typedef struct
	{
		char cabecera[4];
		char tipo;
		hash usuario;
		hash msj;
		unsigned short trozo;
		unsigned short ntrozos;
		char sonido[BUFLEN-TAM_CABECERA-2];
	}mensaje;
};

