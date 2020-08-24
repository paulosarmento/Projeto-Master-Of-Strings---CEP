#include "musica.h"

musica::musica()
{
	timeMs = 0;
	deltaTime = 0;
	temp = 0;
	numNotes = 0;
};

bool musica::carregar(char *arquivo, char *tab)
{
    // Abre o arquivo MIDI
	if( !ReadMidiFile( arquivo, mt ) )
    {
		printf("MIDI: Nao foi possivel carregar a musica %s\n", arquivo);
        return false;
    }
	printf("MIDI: Musica %s carregada com exito\n", arquivo);
	file = fopen( tab, "r" );
	lerNotas();
	return true;
};

void musica::lerNotas()
{
	// Lê o arquivo e armazena as informações da música
	MIDISequencer seq(&mt);
	int n;
	seq.GoToZero();
	while ( seq.GetNextEvent ( &track, &msg ) )
    {
        if ( msg.IsNote() )
        {
            timeMs = seq.GetCurrentTimeInMs();
            if ( msg.IsNoteOn() )
            {
				musicNotes[numNotes].numero = msg.GetNote() - 12;
				musicNotes[numNotes].inicio = timeMs;

				c = fgetc(file);
				fgetc(file);
				musicNotes[numNotes].corda = c - 48;
				c = fgetc(file);
				n = (c - 48) * 10;
				c = fgetc(file);
				n += (c - 48);
				fgetc(file);
				musicNotes[numNotes].casa = n;
            }
            else
            {
				musicNotes[numNotes].fim = timeMs;
                deltaTime = timeMs - temp;
				musicNotes[numNotes].duracao = deltaTime;
				numNotes++;
            }
            temp = timeMs;
        }
    }
	printf("%d notas\n\n", numNotes);
	for ( int i = 0; i < numNotes; i++)
		printf( "Nota: %s Inicio: %g Fim: %g Duracao: %g Corda: %d Casa: %d\n" , note[musicNotes[i].numero], musicNotes[i].inicio, musicNotes[i].fim, musicNotes[i].duracao, musicNotes[i].corda, musicNotes[i].casa );
	
	printf("MIDI: %d notas\n\n", numNotes);
};
