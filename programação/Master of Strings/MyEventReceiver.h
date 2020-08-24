#ifndef _MASTER_OF_STRINGS_EVENT_RECEIVER_
#define _MASTER_OF_STRINGS_EVENT_RECEIVER_

#include "functions.h"

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event);
	void setBotao(s32* bt);
private:
	s32* botaoClicado;
};

#endif