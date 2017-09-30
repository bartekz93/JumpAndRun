#pragma once

#include"MenuState.h"


namespace APPLICATION
{
	class MainMenuState : public MenuState
	{
		enum MENUPOS { NEWGAME, LOAD, HIGHSCORES, OPTIONS, QUIT };
	private:
		RENDERER::Sprite Menu;

		int Pos;
		int NumPos;
		int Choice;
		bool Blocked;
	public:
		MainMenuState() : MenuState(ApplicationState::MAINMENU) {}


		void OnKeyDown(int key);
		void OnKeyUp(int key);

		void OnOpen() { Choice = -1; App->Music->setPitchShift(1.0f); }
		void OnDraw();
		APPSTATE OnUpdate(float dt);
		void OnInit();
		void OnRelease();
	};
}