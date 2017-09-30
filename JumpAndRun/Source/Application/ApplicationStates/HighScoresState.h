#pragma once
#include"MenuState.h"

namespace APPLICATION
{
	struct HSLine
	{
		string Name;
		int Points;

		HSLine(const string& name, int p) : Name(name), Points(p) {}
	};

	struct ScoresSort
	{
		bool operator() (const HSLine& i, const HSLine& j)
		{
			return i.Points > j.Points;
		}
	};

	class HighScoresState : public MenuState
	{
		vector<HSLine> Lines;
	public:
		HighScoresState() : MenuState(ApplicationState::HIGHSCORES) {}

		void Init();
		void OnOpen();
		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }
		APPSTATE OnUpdate(float dt) { return HIGHSCORES; }
		void OnDraw();
		void Release() {}
	};
}
