#include<allegro.h>
#include"../Application.h"
#include"NewGameState.h"

using namespace APPLICATION;


void NewGameState::OnKeyDown(int key)
{
	if (key == KEY_BACKSPACE && UserName.size() > 0)
	{
		UserName.pop_back();
	}
	if (key == KEY_ENTER)
	{
		App->CreateUser(UserName);
		End = true;
	}
}

ApplicationState::APPSTATE NewGameState::OnUpdate(float dt)
{
	if (End) return LEVELCHOICE;

	return NEWGAME;
}

void NewGameState::OnChar(char c)
{
	if (c == ' ') c = '_';
	UserName.push_back(c);
}

void NewGameState::OnDraw()
{
	Vec2 center = RendererObj->GetScreen().GetCenter();

	Rect window = Rect(center.x - 150, center.y - 100, 300, 200);

	RendererObj->DrawTransRect(window, 0x000000, 0.2f);

	RendererObj->DrawString("Podaj imie", center.x - 100, center.y - 50);
	RendererObj->DrawString(UserName, center.x - 100, center.y);
}
