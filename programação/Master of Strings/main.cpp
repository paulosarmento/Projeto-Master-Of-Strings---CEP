#include "functions.h"
#include "musica.h"
#include "som.h"
#include "grafico.h"
#include "cstopwatch.h"

int main ( int argc, char ** argv )
{
	// GRÁFICO
	sistemaGrafico grafico;

	// MIDI
	int numMusicas;
	musica musicas[NUM_MUSICAS];
	s32 musicaSelecionada;

	// Carrega os arquivos MIDI
	for(numMusicas = 0; numMusicas < NUM_MUSICAS; numMusicas++)
		if( !musicas[numMusicas].carregar( listaMusicas[numMusicas], listaPosicoes[numMusicas] ) )
			numMusicas--;

	// SOM
	sistemaSom som;
	bool playMusica = false;
	bool play = false;
	int notaCerta, notaDetectada;

	// Controle do tempo
    CStopWatch hrTimer;


	// LOOP PRINCIPAL
	while( grafico.run() )
    {
		// Começa a contagem do tempo para ajuste do frame rate
		hrTimer.startTimer();


		// Executa as ações do sistema gráfico 3D
		grafico.loop3D();

		// Desenha tudo na tela e encerra o loop 3D
		grafico.fechaLoop3D();

		// Executa as ações do sistema gráfico 2D
		grafico.loop2D();

		// Exibe o menu com as músicas
		if( grafico.getEstado() == ESTADO_MENU )
		{
			musicaSelecionada = grafico.menu(numMusicas);
		}

		if( grafico.getEstado() == ESTADO_JOGO )
		{
			// Exibe a contagem regressiva
			if( !play )
				play = grafico.contagemRegressiva();

			// Toca a música e exibe as notas
			if( play )
			{
				if( !playMusica )
					playMusica = som.carregarSom(listaMusicas[musicaSelecionada]);
				if( grafico.exibeNotas(&musicas[musicaSelecionada].musicNotes[0], musicas[musicaSelecionada].numNotes, &notaCerta) )
					som.tocarMusica();
				else
					som.pararMusica();
				notaDetectada = som.detectarNota();
				grafico.compararNotas( notaCerta, notaDetectada );
			}
		}

		// Desenha tudo na tela e encerra o loop 2D
		grafico.fechaLoop2D();


		// Termina a contagem do tempo e ajusta o frame rate
		hrTimer.stopTimer();
		s16 timeElp = (s16) ( (1.0f / (double)FPS - hrTimer.getElapsedTime() ) * 1000.0f );
		if( timeElp > 0 )
			grafico.sleep( timeElp );
    }

	som.encerrar();
	grafico.encerrar();

    return 0;
};
