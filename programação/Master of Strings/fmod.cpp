/*===============================================================================================
 Master of Strings

 Mistura dos exemplos PlayStream e Pitch detection da FMOD API.
===============================================================================================*/
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "../../api/inc/fmod.hpp"
#include "../../api/inc/fmod_errors.h"

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

static const char *note[120] =
{
    "C 0", "C#0", "D 0", "D#0", "E 0", "F 0", "F#0", "G 0", "G#0", "A 0", "A#0", "B 0",  
    "C 1", "C#1", "D 1", "D#1", "E 1", "F 1", "F#1", "G 1", "G#1", "A 1", "A#1", "B 1",  
    "C 2", "C#2", "D 2", "D#2", "E 2", "F 2", "F#2", "G 2", "G#2", "A 2", "A#2", "B 2",  
    "C 3", "C#3", "D 3", "D#3", "E 3", "F 3", "F#3", "G 3", "G#3", "A 3", "A#3", "B 3",  
    "C 4", "C#4", "D 4", "D#4", "E 4", "F 4", "F#4", "G 4", "G#4", "A 4", "A#4", "B 4",  
    "C 5", "C#5", "D 5", "D#5", "E 5", "F 5", "F#5", "G 5", "G#5", "A 5", "A#5", "B 5",  
    "C 6", "C#6", "D 6", "D#6", "E 6", "F 6", "F#6", "G 6", "G#6", "A 6", "A#6", "B 6",  
    "C 7", "C#7", "D 7", "D#7", "E 7", "F 7", "F#7", "G 7", "G#7", "A 7", "A#7", "B 7",  
    "C 8", "C#8", "D 8", "D#8", "E 8", "F 8", "F#8", "G 8", "G#8", "A 8", "A#8", "B 8",  
    "C 9", "C#9", "D 9", "D#9", "E 9", "F 9", "F#9", "G 9", "G#9", "A 9", "A#9", "B 9"
};

static const float notefreq[120] =
{
      16.35f,   17.32f,   18.35f,   19.45f,    20.60f,    21.83f,    23.12f,    24.50f,    25.96f,    27.50f,    29.14f,    30.87f, 
      32.70f,   34.65f,   36.71f,   38.89f,    41.20f,    43.65f,    46.25f,    49.00f,    51.91f,    55.00f,    58.27f,    61.74f, 
      65.41f,   69.30f,   73.42f,   77.78f,    82.41f,    87.31f,    92.50f,    98.00f,   103.83f,   110.00f,   116.54f,   123.47f, 
     130.81f,  138.59f,  146.83f,  155.56f,   164.81f,   174.61f,   185.00f,   196.00f,   207.65f,   220.00f,   233.08f,   246.94f, 
     261.63f,  277.18f,  293.66f,  311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f, 
     523.25f,  554.37f,  587.33f,  622.25f,   659.26f,   698.46f,   739.99f,   783.99f,   830.61f,   880.00f,   932.33f,   987.77f, 
    1046.50f, 1108.73f, 1174.66f, 1244.51f,  1318.51f,  1396.91f,  1479.98f,  1567.98f,  1661.22f,  1760.00f,  1864.66f,  1975.53f, 
    2093.00f, 2217.46f, 2349.32f, 2489.02f,  2637.02f,  2793.83f,  2959.96f,  3135.96f,  3322.44f,  3520.00f,  3729.31f,  3951.07f, 
    4186.01f, 4434.92f, 4698.64f, 4978.03f,  5274.04f,  5587.65f,  5919.91f,  6271.92f,  6644.87f,  7040.00f,  7458.62f,  7902.13f, 
    8372.01f, 8869.84f, 9397.27f, 9956.06f, 10548.08f, 11175.30f, 11839.82f, 12543.85f, 13289.75f, 14080.00f, 14917.24f, 15804.26f
};

#define OUTPUTRATE          48000
#define SPECTRUMSIZE        8192
#define SPECTRUMRANGE       ((float)OUTPUTRATE / 2.0f)      /* 0 to nyquist */

#define BINSIZE      (SPECTRUMRANGE / (float)SPECTRUMSIZE)


int main(int argc, char *argv[])
{
    FMOD::System          *system        = 0;
    FMOD::Sound           *soundMusica   = 0;
    FMOD::Sound           *soundGuitarra = 0;
    FMOD::Channel         *channelMusica   = 0;
    FMOD::Channel         *channelGuitarra = 0;
    FMOD_RESULT            result;
    FMOD_CREATESOUNDEXINFO exinfo;
    int                    key, soundDriver, recorddriver, numdrivers, count, bin;
    unsigned int           version;    

    /*
        Create a System object
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

	/*
		Seta as saídas e os drivers para gravação
	*/
	soundDriver = 0;
	recorddriver = 0;

	result = system->setOutput(FMOD_OUTPUTTYPE_WINMM);
    ERRCHECK(result);

    result = system->setDriver(soundDriver);
    ERRCHECK(result);

    result = system->setSoftwareFormat(OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 1, 0, FMOD_DSP_RESAMPLER_LINEAR);
    ERRCHECK(result);

	/*
		Inicializa o sistema
	*/
    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

	/*
		Carrega o arquivo de som
	*/
    result = system->createStream("../media/parabens.mid", FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundMusica);
    ERRCHECK(result);

    /*
		Interface
	*/
	printf("====================================================================\n");
    printf("Master of Strings. Projeto FECEP 2011. Clovis, Paulo e Talita - M4AI\n");
    printf("====================================================================\n");
    printf("\n");
    printf("Press space to pause, Esc to quit\n");
    printf("\n");

    /*
        Play the sound.
    */
    result = system->playSound(FMOD_CHANNEL_FREE, soundMusica, true, &channelMusica);
    ERRCHECK(result);

    /*
        Create a sound to record to.
    */
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = 1;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = OUTPUTRATE;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5;
    
    result = system->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &soundGuitarra);
    ERRCHECK(result);

	/*
		Começa a gravação
	*/
	result = system->recordStart(recorddriver, soundGuitarra, true);
    ERRCHECK(result);
    
    Sleep(200);      /* Give it some time to record something */
    
    result = system->playSound(FMOD_CHANNEL_REUSE, soundGuitarra, false, &channelGuitarra);
    ERRCHECK(result);

    /* Dont hear what is being recorded otherwise it will feedback.  Spectrum analysis is done before volume scaling in the DSP chain */
    result = channelGuitarra->setVolume(0);
    ERRCHECK(result);
	
    bin = 0;

	/*
        Main loop.
    */
    do
    {
        if (_kbhit())
        {
            key = _getch();

            switch (key)
            {
                case ' ' :
                {
                    bool paused;
                    channelMusica->getPaused(&paused);
                    channelMusica->setPaused(!paused);
                    break;
                }
                case 'p' : 
                { 
                    FMOD::ChannelGroup* masterGroup = NULL; 
                    system->getMasterChannelGroup(&masterGroup); 
                    if (masterGroup) 
                    { 
                        bool paused; 
                        masterGroup->getPaused(&paused); 
                        masterGroup->setPaused(!paused); 
                    } 
                    break; 
                }
            }
        }

        system->update();

        if (channelMusica)
        {
            unsigned int ms;
            unsigned int lenms;
            bool         playing;
            bool         paused;

            result = channelMusica->isPlaying(&playing);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
            {
                ERRCHECK(result);
            }

            result = channelMusica->getPaused(&paused);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
            {
                ERRCHECK(result);
            }

            result = channelMusica->getPosition(&ms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
            {
                ERRCHECK(result);
            }

            result = soundMusica->getLength(&lenms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
            {
                ERRCHECK(result);
            }

            printf("Time %02d:%02d:%02d/%02d:%02d:%02d %s ", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : playing ? "Playing" : "Stopped");
        }

        /* Detecção da nota */
		static float spectrum[SPECTRUMSIZE];
        float        dominanthz = 0;
        float        max;
        int          dominantnote = 0;
        float        binsize = BINSIZE;

        result = channelMusica->getSpectrum(spectrum, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
        ERRCHECK(result);

        max = 0;

        for (count = 0; count < SPECTRUMSIZE; count++)
        {
            if (spectrum[count] > 0.01f && spectrum[count] > max)
            {
                max = spectrum[count];
                bin = count;
            }
        }

        dominanthz  = (float)bin * BINSIZE;       /* dominant frequency min */

        dominantnote = 0;
        for (count = 0; count < 120; count++)
        {
             if (dominanthz >= notefreq[count] && dominanthz < notefreq[count + 1])
             {
                /* which is it closer to.  This note or the next note */
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

        printf("NtMus %-3s (%7.1f hz) ", note[dominantnote], notefreq[dominantnote]);

		Sleep(10);

        /* Nota do som da guitarra */
		static float spectrumGuitarra[SPECTRUMSIZE];
        float        dominanthzGuitarra = 0;
        float        maxGuitarra;
        int          dominantnoteGuitarra = 0;
        float        binsizeGuitarra = BINSIZE;

		result = channelGuitarra->getSpectrum(spectrumGuitarra, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
        ERRCHECK(result);

        maxGuitarra = 0;

        for (count = 0; count < SPECTRUMSIZE; count++)
        {
            if (spectrumGuitarra[count] > 0.01f && spectrumGuitarra[count] > maxGuitarra)
            {
                maxGuitarra = spectrumGuitarra[count];
                bin = count;
            }
        }

        dominanthzGuitarra  = (float)bin * BINSIZE;       /* dominant frequency min */

        dominantnoteGuitarra = 0;
        for (count = 0; count < 120; count++)
        {
             if (dominanthzGuitarra >= notefreq[count] && dominanthzGuitarra < notefreq[count + 1])
             {
                /* which is it closer to.  This note or the next note */
                if (fabs(dominanthzGuitarra - notefreq[count]) < fabs(dominanthzGuitarra - notefreq[count+1]))
                {
                    dominantnoteGuitarra = count;
                }
                else
                {
                    dominantnoteGuitarra = count + 1;
                }
                break;
             }
        }
        printf("NtGui %-3s (%7.1f hz)", note[dominantnoteGuitarra], notefreq[dominantnoteGuitarra]);

		if(notefreq[dominantnoteGuitarra] == notefreq[dominantnote])
			printf(" ==");
		else
			printf(" !=");

		printf("\r");

		Sleep(10);

    } while (key != 27);

    printf("\n");

    /*
        Shut down
    */
    result = soundMusica->release();
    ERRCHECK(result);
    result = soundGuitarra->release();
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    return 0;
}
