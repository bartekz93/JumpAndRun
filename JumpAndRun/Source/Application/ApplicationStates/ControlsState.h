#pragma once

#include"MenuState.h"
#include"../../Game/Object.h"
#include"../../Game/Game.h"
#include"../../Game/Controls.h"

namespace APPLICATION
{
	class ControlsState : public MenuState
	{
	private:
		int Pos;
		int Choice;
		bool Blocked;
		bool WaitForKey;

		GAME::Game* pGame;
		GAME::Controls Ctrls;

		enum ELEMENTS { LEFT, RIGHT, UP, DOWN, OK, CANCEL };
	public:
		ControlsState() : MenuState(ApplicationState::CONTROLS), WaitForKey(false) {}

		void Create(Game* game) { pGame = game; }
		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }

		void OnKeyDown(int key);
		void OnKeyUp(int key);

		void OnOpen() { Choice = -1; Pos = 0; Ctrls = pGame->GetControls(); }
		void OnDraw();
		APPSTATE OnUpdate(float dt);
		void OnInit() {}
		void OnRelease() {}
	};
}
