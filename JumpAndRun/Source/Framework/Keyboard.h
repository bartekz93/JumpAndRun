#pragma once
#include"KeyboardListener.h"
#include<string>
#include<vector>


namespace FRAMEWORK
{

	class Keyboard
	{
	public:
		bool UpperCase;

		int Keys[127];
		bool KeyState[127];
		std::vector<KeyboardListener*> Listeners;

		void AddListener(KeyboardListener& list);
		void RemoveListener(KeyboardListener* kl);

		static char IsChar(int key);
		static std::string GetKeyName(int key);

		void Update();
		Keyboard();
	};

}