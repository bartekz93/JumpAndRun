#pragma once

namespace FRAMEWORK
{

	class KeyboardListener
	{
		friend class Keyboard;
	public:
		void Listen() { Active = true; }
		void Sleep() { Active = false; }
		KeyboardListener() : Active(true) {}
	protected:
		bool Active;
	
		virtual void OnChar(char c) {}
		virtual void OnKeyDown(int key) {}
		virtual void OnKeyUp(int key) {}
	};

}