#ifndef _MASTER_OF_STRINGS_MUSICA_
#define _MASTER_OF_STRINGS_MUSICA_

#include "functions.h"

class musica
{	
private:
	MIDITimedBigMessage msg;
    MIDIMultiTrack mt;
    int track;
	FILE* file;
	char c;

	// Vari�veis que armazenam os tempos lidos do arquivo MIDI
    double timeMs;
    double deltaTime;
    double temp;

	void lerNotas();

public:
	// Array que armazena as notas da m�sica com seus respectivos tempos de in�cio, fim e dura��o
	nota musicNotes[256];
	// Quantidade de notas na m�sica
	int numNotes;

	musica();
	bool carregar(char *arquivo, char *tab);
};

#endif