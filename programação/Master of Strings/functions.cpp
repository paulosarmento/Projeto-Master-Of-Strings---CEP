#include "functions.h"

int time2frames( double t )
{
	int frames = (int) (( t / (double)(1/(double)(FPS)) ) / 1000.0f);
	return frames;
};

