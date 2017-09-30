#pragma once
#include"MenuState.h"
#include"GameState.h"
#include"../User.h"

namespace APPLICATION
{
	class LevelChoiceState : public MenuState
	{
		friend class Application;
	private:
		GameState* GameState;

		User* CurrentUser;
		int MaxElements;
		int Choice;

		int Pos;
		bool Blocked;
	public:
		LevelChoiceState() : MenuState(ApplicationState::LEVELCHOICE), CurrentUser(NULL), MaxElements(3) {}

		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }
		void OnInit();
		void OnOpen();
		APPSTATE OnUpdate(float dt);
		void OnDraw();
		void OnRelease() {}

		void OnKeyDown(int key);
		void OnKeyUp(int key);
	};
}
