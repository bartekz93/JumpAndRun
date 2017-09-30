#pragma once

#include"Messages.h"
#include"KeyboardListener.h"
#include"Keyboard.h"

using namespace std;

namespace FRAMEWORK
{
	int		modulo(int a, int b);
	int		StrToInt(const string& s);
	string	FloatToStr(float value);
	float	StrToFloat(string str);
	string	IntToStr(int value);
	
	
	class Framework : public MessagesListener
	{
	protected:
		bool End;
		float FPS;
		Keyboard KeyboardObj;
	public:
		Framework() : End(false) {}
	
		void AddKeyboardListener(KeyboardListener* kl);
		void RemoveKeyboardListener(KeyboardListener* kl);
	
		void OnMessage(const Message& msg);
		void Run();
		void Update();
	
		int GetFPS() const { return FPS; }
		Keyboard* GetKeyboard() { return &KeyboardObj; }
	
		virtual void OnRender() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnQuit() {}
	};

};