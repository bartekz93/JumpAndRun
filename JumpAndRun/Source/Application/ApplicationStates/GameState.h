#pragma once
#include"../ApplicationState.h"
#include"../../Game/Game.h"

namespace APPLICATION
{
	class GameState : public ApplicationState
	{
	public:
		enum STATE { GAME, GAMEOVER, GAMESTATS, MENU };
		enum MENU { RETURN, MAINMENU };
	private:
		GAME::Game GameObj;
		STATE State;

		string Map;
		string MenuStr[2];
		string GameOverStr[2];
		string GameStatsStr[2];

		int Level;
		int Pos;
		bool Blocked;
		int Choice;
	public:
		GameState() : ApplicationState(ApplicationState::GAME), State(GAME), Choice(-1) {}

		GAME::Game* GetGame() { return &GameObj; }
		void SetLevel(int l) { Level = l; }
		int GetLevel() { return Level; }

		APPSTATE OnQuit() { State = MENU; return ApplicationState::GAME; }
		void Init();
		void OnOpen();
		APPSTATE Update(float dt);
		void Draw();
		void Release();

		void OnKeyDown(int key);
		void OnKeyUp(int key);
	};
}
