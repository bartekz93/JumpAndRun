#include"Messages.h"

using namespace FRAMEWORK;

MessagesManager FRAMEWORK::MsgManager;

void MessagesManager::AddMessage(Message::TYPE type, const string& msg, int line, const string& file)
{
	Message message(type, line, file, msg);
	for (int i=0;i<Listeners.size();i++)
		Listeners[i]->OnMessage(message);
}

void MessagesManager::AddMessageListener(MessagesListener& listener)
{
	Listeners.push_back(&listener);
}
