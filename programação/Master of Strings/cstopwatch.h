#ifndef _MASTER_OF_STRINGS_WATCH_
#define _MASTER_OF_STRINGS_WATCH_

#include <windows.h>

//CLASSE CSTOPWATCH (CPLUS.ABOUT.COM) (PARA FIXAR O FRAMERATE)
typedef struct
{
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
}stopWatch;

class CStopWatch
{
    private:
        stopWatch timer;
        LARGE_INTEGER frequency;
        double LIToSecs( LARGE_INTEGER & L);
    public:
        CStopWatch();
        void startTimer( );
        void stopTimer( );
        double getElapsedTime();
};

#endif