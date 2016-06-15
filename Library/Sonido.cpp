#include "Sonido.h"

using namespace irrklang;

/*Carga un sonido desde un archivo externo. nomArchivo es una string que le pasamos con el nombre del 
archivo que queremos abrir.*/
Sonido::sonido Sonido::cargarSonidoArchivoExterno(char * nombreArchivo){
	irrklang::ISoundEngine *engine = createIrrKlangDevice();
	irrklang::IAudioRecorder* recorder = createIrrKlangAudioRecorder(engine);
	if (!engine || !recorder)
	{
		printf("Could not create audio engine or audio recoder\n");
		throw 1;
	}
	sonido s;
	// play the recorded audio
	recorder->addSoundSourceFromRecordedAudio(nombreArchivo);
	s.direccion = recorder->getRecordedAudioData(); //Este puntero saca el inicio del buffer donde se guarda el audio.
	s.tam = recorder->getAudioFormat().FrameCount*recorder->getAudioFormat().getFrameSize();

	return s;
}


/*Sobrecarga de método: Graba en archivo externo el resultado de la grabacion
lleva una cadena que se le pasa por parametros para pasarle el nombre 
del archivo que se guardara*/
DLL int Sonido::grabarMensaje(char* nombreArchivo, char grabar){
	static irrklang::ISoundEngine *engine;
	static irrklang::IAudioRecorder* recorder;
	int n = 0;
	//Grabamos audio
	if (grabar==1)
	{
		engine = createIrrKlangDevice();
		recorder = createIrrKlangAudioRecorder(engine);
		if (!engine || !recorder)
		{
			printf("Could not create audio engine or audio recoder\n");
			throw 1;
		}

		recorder->startRecordingBufferedAudio(22000, ESF_U8);
		printf("\nGRABANDO\n");
	}
	else //Dejar de grabar
	{
		recorder->stopRecordingAudio();

		printf("\nGrabacion finalizada, grabados %dms de audio.\n",
			recorder->getAudioFormat().FrameCount * 1000 / recorder->getAudioFormat().SampleRate);//Esta cuenta calcula los milisegundos de grabacion.
		//Grabamos el audio en un archivo externo .wav: 22 kHz, 8bits little endian

		writeWaveFile(nombreArchivo, recorder->getAudioFormat(), recorder->getRecordedAudioData());
		n = recorder->getAudioFormat().FrameCount*recorder->getAudioFormat().getFrameSize();
		recorder->drop();
		engine->drop();
		recorder = NULL;
		engine = NULL;
	}

	return n;
}

/*Sobrecarga de metodo: Graba EN MEMORIA el resultado de la grabacion
Cuando vayas a grabar otro archivo el buffer de memoria donde tiene guardado
el archivo de sonido se limpiará*/
/*Sonido::sonido Sonido::grabarMensaje(){
	sonido s;

	printf("\nPulsa una tecla para comenzar a grabar audio...\n");
	_getch();
	//Grabamos audio
	recorder->startRecordingBufferedAudio();

	printf("\nGRABANDO, pulsa una tecla para parar la grabacion...\n");
	_getch();

	recorder->stopRecordingAudio();
	s.direccion = recorder->getRecordedAudioData(); //Este puntero saca el inicio del buffer donde se guarda el audio.
	s.tam = recorder->getAudioFormat().FrameCount*recorder->getAudioFormat().getFrameSize();
	
	printf("\nGrabacion finalizada, grabados %dms de audio.\n",
	recorder->getAudioFormat().FrameCount * 1000 / recorder->getAudioFormat().SampleRate);//Esta cuenta calcula los milisegundos de grabacion.

	return s;
}*/

/*Funcion utilizada para liberar memoria y limpiar variables.*/
void Sonido::eliminarSonido(sonido s)
{
	free(s.direccion);
	s.direccion = NULL;
	s.tam = 0;
}





/*Funcion que se encarga de grabar un archivo en memoria. Solo se le llama desde
grabarmensaje(nombreArchivo) para guardar el archio de audio grabado en un fichero aparte.*/
void Sonido::writeWaveFile(const char* filename, SAudioStreamFormat format, void* data)
{
	if (!data)
	{
		printf("Could not save recorded data to %s, nothing recorded\n", filename);
		return;
	}

	FILE* file = fopen(filename, "wb");

	if (file)
	{
		// write wave header 
		unsigned short formatType = 1;
		unsigned short numChannels = format.ChannelCount;
		unsigned long  sampleRate = format.SampleRate;
		unsigned short bitsPerChannel = format.getSampleSize() * 8;
		unsigned short bytesPerSample = format.getFrameSize();
		unsigned long  bytesPerSecond = format.getBytesPerSecond();
		unsigned long  dataLen = format.getSampleDataSize();

		const int fmtChunkLen = 16;
		const int waveHeaderLen = 4 + 8 + fmtChunkLen + 8;

		unsigned long totalLen = waveHeaderLen + dataLen; //Tamaño del archivo que estoy buscando?

		fwrite("RIFF", 4, 1, file);
		fwrite(&totalLen, 4, 1, file);
		fwrite("WAVE", 4, 1, file);
		fwrite("fmt ", 4, 1, file);
		fwrite(&fmtChunkLen, 4, 1, file);
		fwrite(&formatType, 2, 1, file);
		fwrite(&numChannels, 2, 1, file);
		fwrite(&sampleRate, 4, 1, file);
		fwrite(&bytesPerSecond, 4, 1, file);
		fwrite(&bytesPerSample, 2, 1, file);
		fwrite(&bitsPerChannel, 2, 1, file);

		// write data

		fwrite("data", 4, 1, file);
		fwrite(&dataLen, 4, 1, file);
		fwrite(data, dataLen, 1, file);

		// finish

		printf("Saved audio as %s\n", filename);
		fclose(file);
	}
	else
		printf("Could not open %s to write audio data\n", filename);
}