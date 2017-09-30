#pragma once
#include"MenuState.h"

namespace APPLICATION
{
	class NewGameState : public MenuState
	{
		string UserName;
		bool End;
	public:
		NewGameState() : MenuState(ApplicationState::NEWGAME), End(false) {}

		void OnKeyDown(int key);
		void OnKeyUp(int key) {}
		void OnChar(char c);
		void OnOpen() { End = false; }

		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }
		void OnDraw();
		APPSTATE OnUpdate(float dt);
		void OnInit() {}
		void OnRelease() {}
	};
}
