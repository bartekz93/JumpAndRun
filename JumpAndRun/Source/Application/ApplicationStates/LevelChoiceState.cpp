#include<allegro.h>
#include"../Application.h"
#include"LevelChoiceState.h"


using namespace APPLICATION;
using namespace FRAMEWORK;

void LevelChoiceState::OnInit()
{
	CurrentUser = App->GetCurrentUser();
	Pos = 0;
}

void LevelChoiceState::OnOpen()
{
	Choice = -1;
}

ApplicationState::APPSTATE LevelChoiceState::OnUpdate(float dt)
{
	if (Choice >= 0)
	{
		return GAME;
	}

	return LEVELCHOICE;
}

void LevelChoiceState::OnDraw()
{
	Vec2 center = RendererObj->GetScreen().GetCenter();
	int w = 300;
	int h = 70;
	int margin = 5;

	int x = center.x - w*0.5f;
	int y = center.y - h*0.5f - MaxElements*0.5f*(h + margin);
	RendererObj->DrawString(CurrentUser->Name, x, y);

	for (int i = 0; i<MaxElements; i++)
	{
		int x = center.x - w*0.5f;
		int y = center.y - h*0.5f - MaxElements*0.5f*(h + margin) + i*(h + margin);

		Rect window = Rect(x, y + 50, w, h);
		RendererObj->DrawTransRect(window, Pos == i ? 0x00ff00 : 0x000000, 0.5f);

		RendererObj->DrawSurface(Game::StarBig, x + 25, y + 75);
		RendererObj->DrawString(IntToStr(CurrentUser->Stats[i].Points)/*+"/"+IntToStr(App->GetLevelDesc(i).Points)*/, x + 57, y + 75);
		//RendererObj->DrawString(IntToStr(CurrentUser->Stats[i].Enemies)+"/"+IntToStr(App->GetLevelDesc(i).Enemies), x+100, y+50);
		RendererObj->DrawSurface(Game::Clock, x + 200, y + 75);
		RendererObj->DrawString(IntToStr(CurrentUser->Stats[i].Time), x + 232, y + 75);
	}
}

void LevelChoiceState::OnKeyUp(int key)
{
	Blocked = false;
}


void LevelChoiceState::OnKeyDown(int key)
{
	if (Blocked) return;

	if (key == KEY_UP) Pos--;
	if (key == KEY_DOWN) Pos++;
	Pos = modulo(Pos, MaxElements);

	if (key == KEY_ENTER)
	{
		Choice = Pos;
		GameState->SetLevel(Pos);
	}


	Blocked = true;
}