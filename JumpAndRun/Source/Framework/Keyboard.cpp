#include<allegro.h>
#include"Keyboard.h"

using namespace FRAMEWORK;
using namespace std;

Keyboard::Keyboard() : UpperCase(false)
{
	for (int i = 0; i<127; i++)
	{
		KeyState[i] = false;
	}
}

string Keyboard::GetKeyName(int key)
{
	char c = IsChar(key);

	if (c != -1)
	{
		if (c == ' ') return "spacja";
		else return string(1, c);
	}

	if (key == KEY_LCONTROL) return "lcontrol";
	if (key == KEY_RCONTROL) return "rcontrol";
	if (key == KEY_LSHIFT) return "lshift";
	if (key == KEY_RSHIFT) return "rshift";
	if (key == KEY_UP) return "strzalka w gore";
	if (key == KEY_DOWN) return "strzalka w dol";
	if (key == KEY_LEFT) return "strzalka w lewo";
	if (key == KEY_RIGHT) return "strzalka w prawo";
	if (key == KEY_TAB) return "tab";
	if (key == KEY_CAPSLOCK) return "capslock";

	return "??";
}

char Keyboard::IsChar(int key)
{
	if (key == KEY_SPACE) return ' ';
	if (key == KEY_A) return 'a';
	if (key == KEY_B) return 'b';
	if (key == KEY_C) return 'c';
	if (key == KEY_D) return 'd';
	if (key == KEY_E) return 'e';
	if (key == KEY_F) return 'f';
	if (key == KEY_G) return 'g';
	if (key == KEY_H) return 'h';
	if (key == KEY_I) return 'i';
	if (key == KEY_J) return 'j';
	if (key == KEY_K) return 'k';
	if (key == KEY_L) return 'l';
	if (key == KEY_M) return 'm';
	if (key == KEY_N) return 'n';
	if (key == KEY_O) return 'o';
	if (key == KEY_P) return 'p';
	if (key == KEY_Q) return 'q';
	if (key == KEY_R) return 'r';
	if (key == KEY_S) return 's';
	if (key == KEY_T) return 't';
	if (key == KEY_U) return 'u';
	if (key == KEY_V) return 'v';
	if (key == KEY_W) return 'w';
	if (key == KEY_X) return 'x';
	if (key == KEY_Y) return 'y';
	if (key == KEY_Z) return 'z';
	if (key == KEY_0) return '0';
	if (key == KEY_1) return '1';
	if (key == KEY_2) return '2';
	if (key == KEY_3) return '3';
	if (key == KEY_4) return '4';
	if (key == KEY_5) return '5';
	if (key == KEY_6) return '6';
	if (key == KEY_7) return '7';
	if (key == KEY_8) return '8';
	if (key == KEY_9) return '9';

	return -1;
}

void Keyboard::Update()
{
	for (int i = 0; i<127; i++)
	{
		if (key[i] && !KeyState[i])
		{
			for (int j = 0; j<Listeners.size(); j++)
			{
				if (!Listeners[j]->Active) continue;

				if (i == KEY_LSHIFT) UpperCase = true;

				char c = IsChar(i);
				if (c > 0)
				{
					Listeners[j]->OnChar(UpperCase ? toupper(c) : c);
				}


				Listeners[j]->OnKeyDown(i);
			}
			KeyState[i] = true;
		}

		if (!key[i] && KeyState[i])
		{
			for (int j = 0; j<Listeners.size(); j++)
			{
				if (!Listeners[j]->Active) continue;

				if (i == KEY_LSHIFT) UpperCase = false;
				if (i == KEY_CAPSLOCK) UpperCase = !UpperCase;

				Listeners[j]->OnKeyUp(i);
			}
			KeyState[i] = false;
		}
	}
}

void Keyboard::AddListener(KeyboardListener& list) 
{ 
	Listeners.push_back(&list); 
}

void Keyboard::RemoveListener(KeyboardListener* kl)
{
	for (int i = 0; i < Listeners.size(); i++)
	{
		if (Listeners[i] == kl)
		{
			Listeners.erase(Listeners.begin() + i);
		}
	}
}