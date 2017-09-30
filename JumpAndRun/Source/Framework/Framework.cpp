#include<allegro.h>
#include<cctype>
#include<sstream>
#include<time.h>
#include"Messages.h"
#include"Framework.h"

using namespace FRAMEWORK;

int FRAMEWORK::modulo(int a, int b)
{
	if (a >= 0) return a%b;
	return (b+a)%b;
}

int	FRAMEWORK::StrToInt(const string& s)
{
	return atoi(s.c_str());
}

string FRAMEWORK::FloatToStr(float value){
	std::ostringstream s;
	s << value;
	return s.str();
}

float FRAMEWORK::StrToFloat(string str){
	return atof(str.c_str());
}

string FRAMEWORK::IntToStr(int value)
{
	string tmp;
	string svalue = itoa(value, (char*)tmp.c_str(), 10); 
	return svalue;
}

void Framework::AddKeyboardListener(KeyboardListener* kl)
{
	KeyboardObj.AddListener(*kl);
}

void Framework::RemoveKeyboardListener(KeyboardListener* kl)
{
	KeyboardObj.RemoveListener(kl);
}

void Framework::OnMessage(const Message& msg)
{
	if (msg.Type == Message::WARN || msg.Type == Message::ERR)
		allegro_message(msg.Text.c_str());
	/*
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)

	int icon;
	string caption;
	switch(msg.Type)
	{
	case Message::INFO:
		icon = MB_ICONINFORMATION;
		caption = "Informacja";
		break;
	case Message::WARN:
		icon = MB_ICONWARNING;
		caption = "Ostrze¿enie";
		break;
	case Message::ERR:
		icon = MB_ICONERROR;
		caption = "B³¹d";
		break;
	}


	if (msg.Type == Message::WARN ) 
	{
		if (MessageBox(0, (msg.Text+"\nPrzerwaæ pracê programu?").c_str(), caption.c_str(), icon | MB_ABORTRETRYIGNORE) == IDABORT)
		{
			exit(0);
		}
	}


	if (msg.Type == Message::ERR) 
	{
		MessageBox(0, msg.Text.c_str(), caption.c_str(), icon);
		MessageBox(0, "nast¹pi wyjœcie z programu", "info", MB_ICONINFORMATION);
		exit(0);
	}
#endif*/
}

void Framework::Run()
{
	while(!End)
	{
		Update();
		OnRender();
	}
}

float GetTime()
{
	return (float)clock()/CLOCKS_PER_SEC;
}

void Framework::Update()
{
	static float Time = GetTime();
	float dt = GetTime() - Time;
	Time = GetTime();

	static int Frames = 0;
	static float TimeSum = 0.0f;

	if (key[KEY_ESC]) OnQuit();//End = true;

	Frames ++;
	TimeSum += dt;

	if (TimeSum >= 1.0f)
	{
		FPS = Frames;
		Frames = 0;
		TimeSum = 0.0f;
	}

	KeyboardObj.Update();
	OnUpdate(dt);
}