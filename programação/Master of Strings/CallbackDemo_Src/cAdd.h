// cAdd.h

#if !defined(CADD_H)
#define CADD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Callback.h"

class cAdd  
{
public:
	void AddNumbers(int Num_A, int Num_B, cCallback *p_CallbackOutput);
	cAdd();
	virtual ~cAdd();

};

#endif // !defined(CADD_H)
