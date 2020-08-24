#include "functions.h"

int main ( int argc, char ** argv )
{

/*** MIDI ***/

	MIDITimedBigMessage msg;
    MIDIMultiTrack mt;
    MIDISequencer seq(&mt);

	// Variáveis que armazenam os tempos lidos do arquivo MIDI
    double timeMs = 0;
    double deltaTime = 0;
    double temp = 0;

	// Array que armazena as notas da música com seus respectivos tempos de início, fim e duração
	nota musicNotes[256];

	int numNotes = 0;
    int track;

/*** SOM ***/

    FMOD::System *system = 0;

	// Canal e som para a música a ser tocada
    FMOD::Sound *soundMusica = 0;
    FMOD::Channel *channelMusica = 0;

	// Canal e som para o áudio captado
    FMOD::Sound *soundGuitarra = 0;
    FMOD::Channel *channelGuitarra = 0;
    FMOD_CREATESOUNDEXINFO exinfo;

    int soundDriver, recorddriver, count;
	int bin = 0;
    unsigned int version;

/*** GRÁFICO ***/

    IrrlichtDevice* device;
    IVideoDriver* driver;

	// Ambiente 3D
    ISceneManager* smgr;
    ISceneNode* pivo;
    ICameraSceneNode* camera;

	// Gráficos 3D
    IMeshSceneNode* cenario;

	// Ambiente 2D
	IGUIEnvironment* guienv;
    IGUIFont* font;

	// Gráficos 2D
	ITexture* traste;
	ITexture* corda;
	ITexture* graficoNota;

	// Controle do tempo
    CStopWatch hrTimer;
	int frame = 0;
	int tempo;
	double intervalo;


/*** MIDI ***/

    // Abre o arquivo MIDI
	if( !ReadMidiFile( "parabens.mid", mt ) )
    {
        printf( "Erro abrindo o arquivo MIDI!" );
        return -1;
    }

	// Lê o arquivo e armazena as informações da música
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
		printf( "nota: %s\tinicio: %g\tfim: %g\tduracao: %g\n" , note[musicNotes[i].numero], musicNotes[i].inicio, musicNotes[i].fim, musicNotes[i].duracao );

/*** SOM ***/

    // Cria o sistema FMOD
    FMOD::System_Create(&system);

	// Checa a versão
    system->getVersion(&version);

    if (version < FMOD_VERSION)
    {
        printf("Erro!  Voce esta usando uma versao antiga do FMOD %08x.  Este programa necessita da versao %08x\n", version, FMOD_VERSION);
        return 0;
    }

	// Seta a saída de som e o driver para gravação
	soundDriver = 0;
	recorddriver = 0;

	system->setOutput(FMOD_OUTPUTTYPE_WINMM);
    system->setDriver(soundDriver);
    system->setSoftwareFormat(OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 1, 0, FMOD_DSP_RESAMPLER_LINEAR);

	// Inicializa o sistema
    system->init(32, FMOD_INIT_NORMAL, 0);

	// Carrega o arquivo de som
    system->createStream("parabens.mid", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundMusica);

	// Toca a música
	system->playSound(FMOD_CHANNEL_FREE, soundMusica, true, &channelMusica);

    // Cria um som para gravação
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels = 1;
    exinfo.format = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = OUTPUTRATE;
    exinfo.length = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;

    system->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &soundGuitarra);

	// Começa a gravação
	system->recordStart(recorddriver, soundGuitarra, true);
	system->playSound(FMOD_CHANNEL_REUSE, soundGuitarra, false, &channelGuitarra);

    // Volume 0 para não dar feedback
    channelGuitarra->setVolume(0);


/*** GRÁFICO ***/

    device = createDevice(EDT_OPENGL, dimension2d<u32>(800,600), 16, false, false, false, 0);
	driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    font = guienv->getBuiltInFont();
	
	// Define o texto da barra de título
	device->setWindowCaption(L"Master of Strings");

	// Luz
	smgr->addLightSceneNode(0, vector3df(10,10,5), SColor(255,255,255,255), 200.0f, 0);

	// Câmera
    pivo = smgr->addEmptySceneNode(0,0);
    camera = smgr->addCameraSceneNode(0, vector3df(100.0f, 50.0f, 100.0f), vector3df(0.0f));
    camera->setFOV(1.0f);
    camera->setParent(pivo);

    // Gráficos 3D
    cenario = smgr->addMeshSceneNode( smgr->getMesh("palco_feira.x") );

	// Gráficos 2D
	graficoNota = driver->getTexture("texturas/bolinha.png");
	traste = driver->getTexture("texturas/traste.png");
	corda = driver->getTexture("texturas/corda.png");

    vector3df rotacao ( -10.0f, -50.0f, 0.0f );
    vector3df posicao ( 0.0f, 10.0f, -125.0f );
    int estadoCamera = 1; // 0->parada, 1->panoramica, 2->centralizar, 3->aproximar
    bool ready2d = false;
    dimension2d<s32> janela;
    int posicaoBolinha = 50;
	stringw text;
	bool play = false;


/*** LOOP PRINCIPAL ***/

    while ( device->run() )
    {
        // Começa a contagem do tempo para ajuste do frame rate
		hrTimer.startTimer();

        // Interface 3d //

        vector3df rotacao ( -11.0f, 0.0f, 0.0f );
        vector3df posicao ( 0.0f, 18.8f, -70.0f );

        pivo->setRotation ( rotacao );
        camera->setPosition ( posicao );

        //if ( estadoCamera == 1 )
        //{
        //    rotacao.Y += 0.5f;
        //    rotacao.X += 0.1f;
        //    if( rotacao.Y <= 0.0f )
        //        posicao.Z += 0.5f;
        //    else
        //        posicao.Z -= 0.5f;

        //    if ( rotacao.Y >= 50 )
        //        estadoCamera = 2;
        //}
        //if ( estadoCamera == 2 )
        //{
        //    rotacao.Y -= 0.5f;
        //    rotacao.X -= 0.1f;
        //    if ( rotacao.Y <= 0 )
        //        estadoCamera = 3;
        //}
        //if ( estadoCamera == 3 )
        //{
        //    posicao.Z += 0.5f;
        //    posicao.Y += 0.08f;
        //    rotacao.X -= 0.1f;
        //    if ( posicao.Z >= -70 )
        //    {
        //        estadoCamera = 0;
                ready2d = true;
        //    }
        //}

        driver->beginScene ( true, true, SColor( 0, 20, 20, 20 ) );
		smgr->drawAll();

        // Interface 2d //

        if ( ready2d )
        {
			// Conta os frames
			frame++;

			// Desenha os gráficos na tela
            janela = driver->getCurrentRenderTargetSize();
            SColor grad1( 255, 153, 102, 51 );
            SColor grad2( 150, 153, 102, 51 );
            driver->draw2DRectangle( rect<s32>( 0, 0, janela.Width, 200 ), grad1, grad1, grad2, grad2 );
            for(int i = 25; i <= 180; i += 30)
			{
				driver->draw2DImage(corda, position2d<s32>(25,i), rect<s32>(0,0,788,11), 0, SColor(255,255,255,255), true);
				driver->draw2DImage(traste, position2d<s32>(10,i-15), rect<s32>(0,0,34,34), 0, SColor(255,255,255,255), true);
			}
            driver->draw2DImage(graficoNota, position2d<s32>(posicaoBolinha++, 50), rect<s32>(0,0,33,33), 0, SColor(255,255,255,255), true);

			// Contagem regressiva
			static int contador = 3;
			if( !(frame % FPS) && !play )
			{
				text = L"Tempo:  ";
				text += contador;
				if( !contador )
				{
					play = true;
					frame = 1;
				}
				else
					contador--;
			}

			// Toca a música e exibe as notas
			static int linha = 10;
			if( play && contador < numNotes )
			{
				nota now = musicNotes[contador];
				nota next = musicNotes[contador + 1];

				bool paused;
				channelMusica->getPaused(&paused);
				if(paused)
					channelMusica->setPaused(false);

				text = L"Nota: ";
				text += note[now.numero];

				tempo = time2frames( now.duracao );

				text += L" - Duracao: ";
				text += tempo;
				text += L" Frames";

				intervalo = next.inicio - now.fim;
				if( intervalo > 0 )
				{
					int t = time2frames( intervalo );
					if( tempo <= frame )
					{
						text = L"Pausa";
						text += L" - Duracao: ";
						text += t;
						text += L" Frames";
					}
					tempo += t;
				}
				if( !(frame % tempo) )
				{
					frame = 0;
					contador++;
					if(linha < 100)
						linha += 10;
					else
						linha = 10;
				}

				// Detecção da nota da guitarra
				static float spectrum[SPECTRUMSIZE];
				float dominanthz = 0;
				float max;
				int dominantnote = 0;
				float binsize = BINSIZE;

				channelGuitarra->getSpectrum(spectrum, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);

				max = 0;

				for (count = 0; count < SPECTRUMSIZE; count++)
				{
					if (spectrum[count] > 0.01f && spectrum[count] > max)
					{
						max = spectrum[count];
						bin = count;
					}
				}

				dominanthz = (float)bin * BINSIZE; // dominant frequency min

				dominantnote = 0;
				for (count = 0; count < 120; count++)
				{
					 if (dominanthz >= notefreq[count] && dominanthz < notefreq[count + 1])
					 {
						// which is it closer to. This note or the next note
						if (fabs(dominanthz - notefreq[count]) < fabs(dominanthz - notefreq[count+1]))
						{
							dominantnote = count;
						}
						else
						{
							dominantnote = count + 1;
						}
						break;
					 }
				}
				text += L" - Nota captada: ";
				text += note[dominantnote];
			}
			font->draw( text, rect<s32>(130,linha,300,50), SColor(255,255,255,255) );
        }

		stringw textFPS;
		textFPS += driver->getFPS();
		font->draw( textFPS, rect<s32>(10,10,300,50), SColor(255,255,255,255) );

        guienv->drawAll();
		driver->endScene();
		system->update();

		// Termina a contagem do tempo e ajusta o frame rate
        hrTimer.stopTimer();
        s16 timeElp = (s16) ( (1.0f / (double)FPS - hrTimer.getElapsedTime() ) * 1000.0f );
        if( timeElp > 0 )
            device->sleep( timeElp, false );
    }

	// Fechamento
    device->drop();
    soundMusica->release();
    soundGuitarra->release();
    system->close();
    system->release();

    return 0;
}
