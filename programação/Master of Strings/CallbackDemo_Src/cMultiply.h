// cMultiply.h


#if !defined(CMULTIPLY_H)
#define CMULTIPLY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Callback.h"

class cMultiply  
{
public:
	void MultiplyNumbers(int Num_A, int Num_B, cCallback *p_CallbackOutput);
	cMultiply();
	virtual ~cMultiply();

};

#endif // !defined(CMULTIPLY_H)
