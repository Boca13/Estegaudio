#include "Red.h"

void Red::dispose(unsigned int s)
{
	closesocket(s);
#ifdef _WIN32
	WSACleanup();
#endif
}


// Envía el mensaje identificado por el hash al usuario dado
char Red::enviaMensaje(unsigned int s, char* ip, char* usuario,char* archivo, char* msj, unsigned short puerto)
{
	struct sockaddr_in si_other;
	int slen = sizeof(sockaddr_in);
	//Rellenar estructura para socket
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(puerto);
	si_other.sin_addr.S_un.S_addr = inet_addr(ip);

	FILE *f = fopen(archivo, "rb");
	if (f == NULL)
		return true;
	fseek(f, 0L, SEEK_END);
	int tam = ftell(f);
	fseek(f, 0L, SEEK_SET);
	unsigned short datagramas = ceil((tam / ((float)BUFLEN - TAM_CABECERA)));
	mensaje m = { { CABECERA[0], CABECERA[1], CABECERA[2], CABECERA[3] }, 'e' };
	m.ntrozos = datagramas;
	unsigned short c;
	int tdatos;
	memcpy(m.msj, msj, TAM_HASH);
	memcpy(m.usuario, usuario, TAM_HASH);

	//Establecer timeout a 15s
	struct timeval tv = { 15000L, 0L }; //ms, us
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
		throw(ERROR);
	}

	for (c = 0; c < datagramas; c++)
	{
		char acked = 0;
		tdatos = fread(m.sonido, 1, sizeof(m.sonido), f); //Antes BUFLEN - TAM_CABECERA
		m.trozo = c;
		do
		{
			if (sendto(s, (char*)&m, TAM_CABECERA + tdatos+1, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
			{
				printf("No se pudo enviar el mensaje.\n");
				fclose(f);
				return true;
			}

			//Esperar ACK
			mensaje ack;
			//try to receive some data, this is a blocking call
			int recv_len;
			recv_len = recvfrom(s, (char*)&ack, sizeof(ack), 0, (struct sockaddr *) &si_other, &slen);
			//Analizar paquete recibido
			if (recv_len > 0)
			{
				if ((ack.cabecera[0] == CABECERA[0]) && (ack.cabecera[1] == CABECERA[1]) && (ack.cabecera[2] == CABECERA[2]) && (ack.cabecera[3] == CABECERA[3]))
				{
					if ((ack.tipo == 'a') && (!memcmp(m.usuario, ack.usuario, TAM_HASH)) && (!memcmp(m.msj, ack.msj, TAM_HASH)))
					{
						acked = 1;
					}
				}
			}
		} while (!acked);
	}
	//Restablecer timeout
	tv.tv_sec = 3600000L; //ms, us
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
		throw(ERROR);
	}
	return false;
}
void Red::recibeMensajes(unsigned int s, void(*funcion)(char *, int, char*))
{
	struct sockaddr_in si_other;
	char buf[BUFLEN];
	int recv_len;
	int slen = sizeof(sockaddr_in);
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//Quitar timeout
		struct timeval tv = { 3600000L, 0L }; //ms, us
		if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
			throw(ERROR);
		}

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			continue;
		}
		else if (recv_len > 0)
		{
			//Analizar paquete recibido
			if ((buf[0] == CABECERA[0]) && (buf[1] == CABECERA[1]) && (buf[2] == CABECERA[2]) && (buf[3] == CABECERA[3]))
			{
				mensaje m;
				if (memcpy(&m, buf, BUFLEN) != NULL)
				{
					if (m.tipo == 'e')
					{
						hash usuario;
						hash msj;
						int esperado = 0;
						if (m.trozo == 0) //Comienza un mensaje
						{
							memcpy(usuario, m.usuario, TAM_HASH);
							memcpy(msj, m.msj, TAM_HASH);
							FILE *f = fopen(m.msj, "wb");

							//Guardar primer trozo
							fwrite(m.sonido, 1, recv_len - TAM_CABECERA, f);
							esperado++;
							//Enviar ACK
							m.tipo = 'a';
							sendto(s, (char*)&m, TAM_CABECERA, 0, (struct sockaddr*) &si_other, slen);

							//Volver a poner timeout
							struct timeval tv = { 15000L, 0L }; //ms, us
							if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
								throw(ERROR);
							}

							while (esperado < m.ntrozos)
							{
								if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
								{
									/*printf("Fallo al recibir paquete.\n");
									fclose(f);
									throw 0;
									return;*/
								}
								else
								{
									if (memcpy(&m, buf, BUFLEN) != NULL)
									{
										if ((!memcmp(usuario, m.usuario, TAM_HASH)) && (!memcmp(msj, m.msj, TAM_HASH)) && (m.trozo == esperado))
										{
											fwrite(m.sonido, 1, recv_len - TAM_CABECERA, f);
											esperado++;
											//Enviar ACK
											m.tipo = 'a';
											sendto(s, (char*)&m, TAM_CABECERA, 0, (struct sockaddr*) &si_other, slen);
										}
									}
								}
							}
							//Mensaje completo recibido:
							int tam = fclose(f);
							funcion(m.msj, tam, m.usuario);

						}
					}
					else if (m.tipo == 'a')
					{

					}
					else
						throw 0;
				}
				else
					throw 0;
			}
		}

	}
}

void Red::recibeMensajes2(unsigned int s)
{
	struct sockaddr_in si_other;
	char buf[BUFLEN];
	int recv_len;
	int slen = sizeof(sockaddr_in);
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//Quitar timeout
		struct timeval tv = { 3600000L, 0L }; //ms, us
		if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
			throw(ERROR);
		}

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
		{
			continue;
		}
		else if (recv_len > 0)
		{
			//Analizar paquete recibido
			if ((buf[0] == CABECERA[0]) && (buf[1] == CABECERA[1]) && (buf[2] == CABECERA[2]) && (buf[3] == CABECERA[3]))
			{
				mensaje m;
				if (memcpy(&m, buf, BUFLEN) != NULL)
				{
					if (m.tipo == 'e')
					{
						hash usuario;
						hash msj;
						int esperado = 0;
						if (m.trozo == 0) //Comienza un mensaje
						{
							//char ruta[255] = "";
							memcpy(usuario, m.usuario, TAM_HASH);
							memcpy(msj, m.msj, TAM_HASH);
							//sprintf(ruta, "/mensajes/recibidos/%s", m.msj);
							FILE *f = fopen("recibido", "wb");
							if (f == NULL)
								throw(ERROR_FILE_INVALID);
							//Guardar primer trozo
							fwrite(m.sonido, 1, recv_len - TAM_CABECERA-1, f);
							esperado++;
							//Enviar ACK
							m.tipo = 'a';
							sendto(s, (char*)&m, TAM_CABECERA, 0, (struct sockaddr*) &si_other, slen);

							//Volver a poner timeout
							struct timeval tv = { 15000L, 0L }; //ms, us
							if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
								throw(ERROR);
							}

							while (esperado < m.ntrozos)
							{
								memset(buf, '\0', BUFLEN);
								if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
								{
									/*printf("Fallo al recibir paquete.\n");
									fclose(f);
									throw 0;
									return;*/
								}
								else
								{
									if (memcpy(&m, buf, BUFLEN) != NULL)
									{
										if ((!memcmp(usuario, m.usuario, TAM_HASH)) && (!memcmp(msj, m.msj, TAM_HASH)) && (m.trozo == esperado))
										{
											fwrite(m.sonido, 1, recv_len - TAM_CABECERA-1, f);
											esperado++;
											//Enviar ACK
											m.tipo = 'a';
											sendto(s, (char*)&m, TAM_CABECERA, 0, (struct sockaddr*) &si_other, slen);
										}
									}
								}
							}
							//Mensaje completo recibido:
							int tam = fclose(f);
							FILE *us=fopen("recibido_usuario", "w");
							if (us != NULL)
							{
								fprintf(us,"%s",m.usuario);
							}
							else
								throw(ERROR_FILE_INVALID);
							_fcloseall();

						}
					}
					else if (m.tipo == 'a')
					{

					}
					else
						throw 0;
				}
				else
					throw 0;
			}
		}

	}
}

unsigned int Red::configurarSocketEnvio()
{
	struct sockaddr_in server;
	unsigned int s;
#ifdef _WIN32
	WSADATA wsa;

	//Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Fallo al inicializar el socket: %d", WSAGetLastError());
		throw(EXIT_FAILURE);
	}
	printf("Socket inicializado.\n");
#else
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("No se pudo crear el socket UDP.\n");
		throw(EXIT_FAILURE);
	}
	printf("Socket creado.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind ha fallado.\n");
		throw(EXIT_FAILURE);
	}
	puts("Bind hecho.\n");
	struct timeval tv = { 15000L, 0L }; //ms, us
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
		throw(ERROR);
	}
	return s;
}

// Primitiva que utilizará cada instancia de chat para configurar un socket por el que recibir mensajesvoip. Sólamente cuando no se inicialice la clase.
unsigned int Red::configurarSocket(char* ipaddr, unsigned short puerto)
{
	struct sockaddr_in server;
	unsigned int s;
#ifdef _WIN32
	WSADATA wsa;

	//Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Fallo al inicializar el socket: %d", WSAGetLastError());
		throw(EXIT_FAILURE);
	}
	printf("Socket inicializado.\n");
#else
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif
	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("No se pudo crear el socket UDP.\n");
		throw(EXIT_FAILURE);
	}
	printf("Socket creado.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(puerto);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind ha fallado.\n");
		closesocket(s);
		exit(1);
	}
	puts("Bind hecho.\n");
	struct timeval tv = { 15000L, 0L }; //ms, us
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
		throw(ERROR);
	}
	return s;
}