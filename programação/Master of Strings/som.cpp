#include "som.h"

sistemaSom::sistemaSom()
{
    system = 0;
    soundMusica = 0;
    channelMusica = 0;
    soundGuitarra = 0;
    channelGuitarra = 0;
	bin = 0;

    // Cria o sistema FMOD
    FMOD::System_Create(&system);

	// Checa a versão
    system->getVersion(&version);

    if (version < FMOD_VERSION)
    {
		printf("SOM: Erro! Voce esta usando uma versao antiga do FMOD %08x. Este programa necessita da versao %08x\n", version, FMOD_VERSION);
    }

	// Seta a saída de som e o driver para gravação como os defaults do sistema
	soundDriver = 0;
	recorddriver = 0;
	system->setOutput(FMOD_OUTPUTTYPE_WINMM);
    system->setDriver(soundDriver);
    system->setSoftwareFormat(OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 1, 0, FMOD_DSP_RESAMPLER_LINEAR);

	// Inicializa o sistema
    system->init(32, FMOD_INIT_NORMAL, 0);

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
};

sistemaSom::~sistemaSom()
{
    //soundMusica->release();
    //soundGuitarra->release();
    //system->close();
    //system->release();
};

bool sistemaSom::carregarSom(char *arquivo)
{
    result = system->createStream(arquivo, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundMusica);
	if(result != FMOD_OK)
	{
		printf("SOM: Nao foi possivel carregar a musica %s\n", arquivo);
		return false;
	}
	printf("SOM: Musica %s carregada com exito\n", arquivo);
	return true;
};

bool sistemaSom::tocarMusica()
{
	bool playing;
	channelMusica->isPlaying(&playing);
	if( !playing )
	{
		channelMusica->setLoopCount(0);
		result = system->playSound(FMOD_CHANNEL_FREE, soundMusica, false, &channelMusica);
	}
	return playing ? playing : result == FMOD_OK;
};

void sistemaSom::pararMusica()
{
	channelMusica->stop();
};

int sistemaSom::detectarNota()
{
	system->update();

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
	return dominantnote;
};

void sistemaSom::encerrar()
{
    soundMusica->release();
    soundGuitarra->release();
    system->close();
    system->release();
};
