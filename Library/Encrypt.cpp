#include "Encrypt.h"


Encrypt::Encrypt()
{
}


Encrypt::~Encrypt()
{
}


// Encripta los datos en memoria y devuelve el puntero al resultado y el tamaño de la información (por referencia).

void* Encrypt::encriptar(void* memoria, int tam, int* tamres)
{
	char algo[] = "rijndael-256";											//Algoritmo a utilizar
	char mode[] = "cbc";													//Modo bloque (requerido por rijndael)
	char key[] = "AESD-CBED-POKF-OHFB-SNCJ-POLD-OK";						//Clave
	char *block_buffer;														//Buffer temporal
	int blocksize;															//Tamanio de bloque

	//No hay fopen de 'memoria', supuestamente ya abierto
	FILE * salida = fopen("salida", "w+");									//Fichero encriptado

	MCRYPT td = mcrypt_module_open(algo, NULL, mode, NULL);					//Carga del modulo
	if (td == MCRYPT_FAILED) {
		printf("Error abriendo la libreria mcrypt\n");
	}

	blocksize = mcrypt_enc_get_block_size(td);								//Tamanio de bloque

	block_buffer = (char*)malloc(blocksize);								//El cast es por la conversion de void* a char*
	//que C++ no hace automatica
	char *iv = (char*)malloc(blocksize);														//IV es el primer bloque (aleatorio)
	for (int i = 0; i < blocksize; i++){
		iv[i] = 'a';
	}

	int r = mcrypt_generic_init(td, key, strlen(key)*sizeof(char), iv);		//Inicializacion del modulo

	int leidos;																//Numero de Bytes leidos a codificar
	int flag = 0;															//Marca un bloque incompleto (menor de blocksize)
	//Es decir, el último.
	while (flag == 0) {
		leidos = fread(block_buffer, 1, blocksize, memoria);				//Se guarda en buffer un bloque de tam blocksize
		if (leidos != blocksize)
		{
			//Caso de que tam leido < blocksize
			flag++;															//Seguimos teniendo la necesidad de enviar un 
			int diff = blocksize - leidos;									//Bloque completo de tam blocksize
			for (int j = diff; j > 0; j--) {								//Rellenamos con 0 hasta completar el bloque
				block_buffer[blocksize - j] = 0;
			}
		}
		mcrypt_generic(td, block_buffer, blocksize);						//Encriptamos el buffer
		fwrite(block_buffer, 1, blocksize, salida);							//Se escribe en la salida
	}

	mcrypt_generic_deinit(td);												//Finalizar modulo
	mcrypt_module_close(td);												//Cerrar modulo

	fclose(salida);
	return NULL;
}

// Análogo desde fichero
void Encrypt::encriptar(char* origen, char* destino)
{
	char algo[] = "rijndael-256";											//Algoritmo a utilizar
	char mode[] = "cbc";													//Modo bloque (requerido por rijndael)
	char key[] = "AESD-CBED-POKF-OHFB-SNCJ-POLD-OK";						//Clave
	char *block_buffer;														//Buffer temporal
	int blocksize;															//Tamanio de bloque

	FILE * textoplano = fopen(origen, "r");									//Fichero a encriptar
	FILE * salida = fopen(destino, "w+");									//Fichero encriptado

	MCRYPT td = mcrypt_module_open(algo, NULL, mode, NULL);					//Carga del modulo
	if (td == MCRYPT_FAILED) {
		printf("Error abriendo la libreria mcrypt\n");
	}

	blocksize = mcrypt_enc_get_block_size(td);

	block_buffer = (char*)malloc(blocksize);								//el cast es por la conversion de void* a char*
	char *iv = (char*)malloc(blocksize);									//que c++ no hace automatica
	for (int i = 0; i < blocksize; i++){
		iv[i] = 'a';														//IV es el primer bloque (aleatorio)
	}

	int r = mcrypt_generic_init(td, key, strlen(key)*sizeof(char), iv);		//Inicia el modulo

	int leidos;																//Numero de Bytes leidos a codificar
	int flag = 0;															//Marca un bloque incompleto (menor de blocksize)
	//Es decir, el último.

	while (flag == 0) {
		leidos = fread(block_buffer, 1, blocksize, textoplano);				//Se guarda en buffer un bloque de tam blocksize
		if (leidos != blocksize)
		{
			flag++;															//Caso de que tam leido < blocksize
			int diff = blocksize - leidos;									//Seguimos teniendo la necesidad de enviar un 
			for (int j = diff; j > 0; j--) {								//Bloque completo de tam blocksize
				block_buffer[blocksize - j] = 0;							//Rellenamos con 0 hasta completar el bloque
			}
		}
		mcrypt_generic(td, block_buffer, blocksize);						//Encriptamos el buffer
		fwrite(block_buffer, 1, blocksize, salida);							//Se escribe en la salida
	}

	mcrypt_generic_deinit(td);												//Finalizar modulo
	mcrypt_module_close(td);												//Cerrar modulo

	fclose(textoplano);														//Cerramos fichero
	fclose(salida);
}


//Desencripta los datos en memoria y devuelve el puntero al resultado y el tamaño de la información (por referencia).
void* Encrypt::desencriptar(void* memoria, int tam, int* tamres)
{
	char algo[] = "rijndael-256";
	char mode[] = "cbc";
	char key[] = "AESD-CBED-POKF-OHFB-SNCJ-POLD-OK";
	char *block_buffer;
	int blocksize;

	//No hay fopen de memoria, supuestamente ya abierto
	FILE * salida = fopen("descodificado.aes", "w+");						//Fichero desencriptado

	MCRYPT td = mcrypt_module_open(algo, NULL, mode, NULL);					//carga el modulo

	if (td == MCRYPT_FAILED) {
		printf("Error abriendo la libreria mcrypt\n");
	}

	blocksize = mcrypt_enc_get_block_size(td);

	block_buffer = (char*)malloc(blocksize);								//el cast es por la conversion de void* a char*
	char *iv = (char*)malloc(blocksize);									//que c++ no hace automatica

	for (int i = 0; i < blocksize; i++){										//IV es el primer bloque (aleatorio)
		iv[i] = 'a';
	}

	int r = mcrypt_generic_init(td, key, strlen(key)*sizeof(char), iv);		//inicia el modulo

	while (fread(block_buffer, 1, blocksize, codificado) != 0) {			//lee el fichero encriptado a bloques
		mdecrypt_generic(td, block_buffer, blocksize);						//se desencripta el buffer
		fwrite(block_buffer, 1, blocksize, salida);							//se escribe en la salida
	}

	mcrypt_generic_deinit(td);												//finaliza el modulo
	mcrypt_module_close(td);												//cierre modulo

	fclose(salida);															//cierre ficheros

	return NULL;
}

void Encrypt::desencriptar(char* origen, char* destino)
{
	char algo[] = "rijndael-256";
	char mode[] = "cbc";
	char key[] = "AESD-CBED-POKF-OHFB-SNCJ-POLD-OK";
	char *block_buffer;
	int blocksize;

	FILE * codificado = fopen(origen, "r");									//Fichero a desencriptar
	FILE * salida = fopen(destino, "w+");						//Fichero desencriptado

	MCRYPT td = mcrypt_module_open(algo, NULL, mode, NULL);					//abre el modulo

	if (td == MCRYPT_FAILED) {
		printf("Error abriendo la libreria mcrypt\n");
	}

	blocksize = mcrypt_enc_get_block_size(td);

	block_buffer = (char*)malloc(blocksize);								//el cast es por la conversion de void* a char*
	char *iv = (char*)malloc(blocksize);									//que c++ no hace automatica

	for (int i = 0; i < blocksize; i++){										//IV es el primer bloque (aleatorio)
		iv[i] = 'a';
	}

	int r = mcrypt_generic_init(td, key, strlen(key)*sizeof(char), iv);		//inicia el modulo

	while (fread(block_buffer, 1, blocksize, codificado) != 0) {			//lee el fichero encriptado a bloques
		mdecrypt_generic(td, block_buffer, blocksize);						//se desencripta el buffer
		fwrite(block_buffer, 1, blocksize, salida);							//se escribe en la salida
	}

	mcrypt_generic_deinit(td);												// se finaliza el modulo
	mcrypt_module_close(td);												// se cierra el modulo

	fclose(codificado);														//se cierran los ficheros
	fclose(salida);
}