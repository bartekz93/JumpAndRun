#include<allegro.h>
#include<algorithm>
#include"../Application.h"
#include"HighScoresState.h"
#include"../User.h"

using namespace APPLICATION;
using namespace FRAMEWORK;


void HighScoresState::Init() {}



void HighScoresState::OnOpen()
{
	User us;
	Lines.clear();
	for (int i = 0; i<App->Users.size(); i++)
	{
		us.Load(App->Users[i]);

		int p = 0;
		for (int j = 0; j<3; j++)
		{
			p += us.Stats[j].Points;
		}

		Lines.push_back(HSLine(App->Users[i], p));
	}

	ScoresSort ScoresSortObj;
	sort(Lines.begin(), Lines.end(), ScoresSortObj);
}

void HighScoresState::OnDraw()
{
	Vec2 c = RendererObj->GetScreen().GetCenter();
	int x = c.x - 150;
	int y = c.y - 150;

	for (int i = 0; i<Lines.size(); i++)
	{
		Rect r = Rect(x, y + 50 * i, 300, 50);
		RendererObj->DrawTransRect(r, 0x000000, 0.5f);
		RendererObj->DrawString(Lines[i].Name, x, y + 50 * i);


		RendererObj->DrawString(IntToStr(Lines[i].Points), x + 200, y + 50 * i);
	}
}