#pragma once
#include<string>
#include<vector>

using namespace std;

namespace FRAMEWORK
{

	class Message
	{
	public:
		enum TYPE { INFO, ERR, WARN };
	
		TYPE Type;
		int Line;
		string File;
		string Text;
	
		Message(TYPE t, int l, const string& f, const string& txt) : Type(t), Line(l), File(f), Text(txt) {}
	};
	
	class MessagesListener
	{
	friend class MessagesManager;
	protected:
		virtual void OnMessage(const Message& msg) = 0;
	};
	
	class MessagesManager
	{
	private:
		vector<MessagesListener*> Listeners;
	public:
	
		void AddMessage(Message::TYPE type, const string& msg, int line, const string& file);
		void AddMessageListener(MessagesListener& listener);
	};
	
	extern MessagesManager MsgManager;
	
	#define INFO(text) MsgManager.AddMessage(Message::INFO, text, __LINE__, __FILE__)
	#define WARNING(text) MsgManager.AddMessage(Message::WARN, text, __LINE__, __FILE__)
	#define ERROR(text)	  MsgManager.AddMessage(Message::ERR,  text, __LINE__, __FILE__)

};