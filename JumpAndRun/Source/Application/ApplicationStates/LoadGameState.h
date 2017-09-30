#pragma once
#include"MenuState.h"

namespace APPLICATION
{

	class LoadGameState : public MenuState
	{
	private:
		MATH::Vec2 Pos;
		MATH::Vec2 Choice;

		int Columns;
		int Rows;
		int NumPos;
		bool Blocked;

	public:
		LoadGameState() : MenuState(ApplicationState::LOADGAME) {}

		int GetUserIndex(const MATH::Vec2& v);

		void OnOpen();
		void OnDraw();
		void OnInit();
		void OnRelease() {}
		void OnKeyDown(int key);
		void OnKeyUp(int key);

		APPSTATE OnUpdate(float dt);
		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }
	};
}