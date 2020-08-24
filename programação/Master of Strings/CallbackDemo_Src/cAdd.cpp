// cAdd.cpp

#include "stdafx.h"
#include "cAdd.h"


cAdd::cAdd()
{
}

cAdd::~cAdd()
{
}

void cAdd::AddNumbers(int Num_A, int Num_B, cCallback *p_CallbackOutput)
{
	char s8_Out[50];
	sprintf(s8_Out, "Adding      %3d + %3d = %3d", Num_A, Num_B, Num_A + Num_B);
	
	p_CallbackOutput->Execute((void*)s8_Out);
}
