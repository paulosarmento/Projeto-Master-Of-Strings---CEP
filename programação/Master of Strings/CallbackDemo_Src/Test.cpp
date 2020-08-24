// Test.cpp

#include "stdafx.h"
#include "Test.h"
#include "cAdd.h"
#include "cMultiply.h"

cTest::cTest()
{
}

cTest::~cTest()
{
}

/*************************** Main Test *************************/

void cTest::TestStart()
{
	printf("> Hello here the great demo begins !\n");	

	cAdd      i_Add;   // local instances
	cMultiply i_Multi;

	// -------------- Demo 1 : normal callback ----------------------

	printf("\n> Setting CallbackOutputStars !\n");

	i_CallbackOutput.SetCallback(this, &cTest::CallbackOutputStars);

	i_Add.AddNumbers       (120, 30, &i_CallbackOutput);
	i_Multi.MultiplyNumbers(3,   22, &i_CallbackOutput);

	// -------------- Demo 2 : changing the callback ----------------

	printf("\n> Setting CallbackOutputDollar !\n");
	
	i_CallbackOutput.SetCallback(this, &cTest::CallbackOutputDollar);

	i_Add.AddNumbers       (33, 66, &i_CallbackOutput);
	i_Multi.MultiplyNumbers(5,  10, &i_CallbackOutput);

	// -------------- Demo 3 : global callback ----------------------

	printf("\n> Finally the callback from a global function :\n");
	
	// don't forget the "extern" in the Test.h file !!
	globalCallback((void*) &i_CallbackOutput);

	printf("\n> Now the Show is over ! Hope you liked it !\n\n> Elmue\n> elmue@gmx.de\n\n");	
}


/************************* Callback Functions **************************/

bool cTest::CallbackOutputStars(void *Param)
{
	char s8_Out[200];
	sprintf(s8_Out, "  ***** Some stars *****  %s\n", (char*) Param);

	printf(s8_Out);
	return true;
}

bool cTest::CallbackOutputDollar(void *Param)
{
	char s8_Out[200];
	sprintf(s8_Out, "  $$$ Now with Dollar $$$ %s\n", (char*) Param);

	printf(s8_Out);
	return true;
}

