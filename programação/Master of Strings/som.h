#ifndef _MASTER_OF_STRINGS_SOM_
#define _MASTER_OF_STRINGS_SOM_

#include "functions.h"

class sistemaSom
{
private:
    FMOD::System *system;

	// Canal e som para a música a ser tocada
    FMOD::Sound *soundMusica;
    FMOD::Channel *channelMusica;

	// Canal e som para o áudio captado
    FMOD::Sound *soundGuitarra;
    FMOD::Channel *channelGuitarra;
    FMOD_CREATESOUNDEXINFO exinfo;

    int soundDriver, recorddriver, count;
	int bin;
	FMOD_RESULT result;
	unsigned int version;

public:
	sistemaSom();
	~sistemaSom();
	bool carregarSom(char *arquivo);
	bool tocarMusica();
	void pararMusica();
	int detectarNota();
	void encerrar();
};

#endif