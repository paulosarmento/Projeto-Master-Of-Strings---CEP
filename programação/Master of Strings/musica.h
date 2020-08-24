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

	// Variáveis que armazenam os tempos lidos do arquivo MIDI
    double timeMs;
    double deltaTime;
    double temp;

	void lerNotas();

public:
	// Array que armazena as notas da música com seus respectivos tempos de início, fim e duração
	nota musicNotes[256];
	// Quantidade de notas na música
	int numNotes;

	musica();
	bool carregar(char *arquivo, char *tab);
};

#endif