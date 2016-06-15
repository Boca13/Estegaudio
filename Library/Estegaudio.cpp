#include "Estegaudio.h"


/*
		Los datos se ocultan en el bit menos significativo de cada byte del sonido.
		Primero se incluye una "cabecera" que indica la longitud del archivo incluido.

		|	Tamaño	|	Archivo	|
		(u32)
		*/


int Estegaudio::introducir(char* archivo, char* sonido, char* destino)
{
	struct
	{
		unsigned char bit : 1;
	}muestra;

	struct
	{
		unsigned char b0 : 1;
		unsigned char b1 : 1;
		unsigned char b2 : 1;
		unsigned char b3 : 1;
		unsigned char b4 : 1;
		unsigned char b5 : 1;
		unsigned char b6 : 1;
		unsigned char b7 : 1;
	}bits;


	//Abrir archivos:
	FILE *f = fopen(archivo, "rb");
	FILE *fs = fopen(sonido, "rb");
	FILE *fd = fopen(destino, "wb");
	if (f&&fs&&fd)
	{
		//Comprobar si cabe el archivo en el sonido; 1 bit de datos por cada byte de sonido
		fseek(f, 0L, SEEK_END);
		fseek(fs, 0L, SEEK_END);
		unsigned int tArchivo = ftell(f);
		unsigned int tSonido = ftell(fs);
		
		char b[OFFSET];
		rewind(f);
		rewind(fs);

		fread(b, OFFSET, 1, fs);
		fwrite(b, OFFSET, 1, fd);

		if ((4 + tArchivo) * 8 + OFFSET < tSonido) //Cabe
		{
			//Comenzar:
			unsigned int cArchivo = 0;
			unsigned int cSonido = 0;

			//Introducir cabecera:
			char *p = (char*)&tArchivo;
			char c;
			for (c = 0; c < 4; c++)
			{
				*((char*)&bits) = p[c];
				//Bit 0
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b0;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 1
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b1;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 2
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b2;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 3
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b3;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 4
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b4;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 5
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b5;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 6
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b6;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;
				//Bit 7
				if (fread((char*)&muestra, 1, 1, fs) == 1)
				{
					muestra.bit = bits.b7;
					if (fwrite(&muestra, 1, 1, fd) != 1)
						break;
				}
				else
					break;

			}

			//Introducir datos
			while (cArchivo <= tArchivo)
			{
				if (fread((char*)&bits, 1, 1, f) == 1)
				{
					//Bit 0
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b0;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 1
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b1;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 2
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b2;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 3
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b3;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 4
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b4;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 5
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b5;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 6
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b6;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;
					//Bit 7
					if (fread((char*)&muestra, 1, 1, fs) == 1)
					{
						muestra.bit = bits.b7;
						if (fwrite(&muestra, 1, 1, fd) != 1)
							break;
					}
					else
						break;

				}
				cArchivo++;
			}
			char b;
			while (fread(&b, 1, 1, fs))
				fwrite(&b, 1, 1, fd);
		}
		else
		{
			fprintf(stderr, "Error: El archivo no cabe en el sonido.\n");
		}
		fclose(f);
		fclose(fd);
		fclose(fs);
	}


	return 0;
}

int Estegaudio::extraer(char* sonido, char* destino)
{
	struct
	{
		unsigned char bit : 1;
	}muestra;

	struct
	{
		unsigned char b0 : 1;
		unsigned char b1 : 1;
		unsigned char b2 : 1;
		unsigned char b3 : 1;
		unsigned char b4 : 1;
		unsigned char b5 : 1;
		unsigned char b6 : 1;
		unsigned char b7 : 1;
	}bits;

	//Abrir archivos:
	FILE *f = fopen(destino, "wb");
	FILE *fs = fopen(sonido, "rb");
	if (f&&fs)
	{
		unsigned int cArchivo = 0; //Tamaño (cabecera)
		unsigned int c;
		char *pcArchivo = (char*)&cArchivo;
		fseek(fs, OFFSET, SEEK_SET);

		//Sacar cabecera
		for (c = 0; c < 4; c++)
		{
			char* p = (char*)&bits;
			//Bit 0
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b0 = muestra.bit;
			//Bit 1
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b1 = muestra.bit;
			//Bit 2
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b2 = muestra.bit;
			//Bit 3
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b3 = muestra.bit;
			//Bit 4
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b4 = muestra.bit;
			//Bit 5
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b5 = muestra.bit;
			//Bit 6
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b6 = muestra.bit;
			//Bit 7
			if (fread(&muestra, 1, 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b7 = muestra.bit;
			pcArchivo[c] = *p;
		}

		//Sacar archivo
		for (c = 0; c < cArchivo; c++)
		{
			//Bit0
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b0 = muestra.bit;

			//Bit1
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b1 = muestra.bit;

			//Bit2
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b2 = muestra.bit;

			//Bit3
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b3 = muestra.bit;

			//Bit4
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b4 = muestra.bit;

			//Bit5
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b5 = muestra.bit;

			//Bit6
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b6 = muestra.bit;

			//Bit7
			if (fread(&muestra, sizeof(muestra), 1, fs) != 1)
				{fclose(f); fclose(fs); return 1;}
			bits.b7 = muestra.bit;


			if (fwrite(&bits, sizeof(bits), 1, f) != 1)
				{fclose(f); fclose(fs); return 1;}
		}

		printf("Tam: %u\n", cArchivo);
		fclose(f);
		fclose(fs);
		return cArchivo;
	}
	return 0;
}