#include "MyEventReceiver.h"

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();

		switch(event.GUIEvent.EventType)
		{
			case EGET_BUTTON_CLICKED:
			{
				*botaoClicado = id;
				return true;
				break;
			}
			default:
			{
				*botaoClicado = -1;
				break;
			}
		}
	}

	return false;
};

void MyEventReceiver::setBotao(s32* bt)
{
	botaoClicado = bt;
};