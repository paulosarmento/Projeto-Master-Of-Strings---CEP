// Test.h

#if !defined(TEST_H)
#define TEST_H

#include "callback.h"


// globalCallback is in global namespace
extern void globalCallback(void* Argument);


class cTest  
{
public:
	void TestStart();

	cTest();
	virtual ~cTest();

private:
	bool CallbackOutputStars(void *Param);
	bool CallbackOutputDollar(void *Param);

	TCallback<cTest> i_CallbackOutput;
};

#endif // !defined(TEST_H)
