#include<allegro.h>
#include"../Application.h"
#include"LoadGameState.h"

using namespace APPLICATION;
using namespace FRAMEWORK;

void LoadGameState::OnInit()
{

}

void LoadGameState::OnOpen()
{
	Choice = Vec2(-1, -1);
	Pos = Vec2(0, 0);
	App->LoadUsers();

	Rows = (RendererObj->GetScreen().h - 10 - LogoRect.y - LogoRect.h) / 50;
	Columns = ceil((double)App->Users.size() / Rows);
}

void LoadGameState::OnDraw()
{
	Vec2 c = RendererObj->GetScreen().GetCenter();


	for (int j = 0; j<Columns; j++)
	{
		for (int i = 0; i<Rows; i++)
		{
			int userIndex = j*Rows + i;
			if (App->Users.size() <= userIndex) return;

			int x = c.x - Columns * 100 + j * 200;
			int y = LogoRect.y + LogoRect.h + i * 50;
			Rect r = Rect(x, y, 200, 50);

			RendererObj->DrawTransRect(r, (i == Pos.y && j == Pos.x) ? 0x00ff00 : 0x000000, 0.5f);
			RendererObj->DrawString(App->Users[userIndex], x, y);
		}
	}
}

void LoadGameState::OnKeyUp(int key)
{
	Blocked = false;
}


void LoadGameState::OnKeyDown(int key)
{
	if (Blocked) return;

	Vec2 prevPos = Pos;
	if (key == KEY_UP) Pos.y--;
	if (key == KEY_DOWN) Pos.y++;
	if (key == KEY_LEFT) Pos.x--;
	if (key == KEY_RIGHT) Pos.x++;

	int upos = modulo(GetUserIndex(Pos), App->Users.size());
	Pos.x = upos / Rows;
	Pos.y = upos - Pos.x*Rows;

	if (GetUserIndex(Pos) < 0) Pos = prevPos;


	if (key == KEY_ENTER)
	{
		Choice = Pos;
	}


	Blocked = true;
}

int LoadGameState::GetUserIndex(const Vec2& v)
{
	int index = v.x*Rows + v.y;
	return index;// < App->Users.size() ? index : -1;
}

ApplicationState::APPSTATE LoadGameState::OnUpdate(float dt)
{
	if (Choice != Vec2(-1, -1))
	{
		int index = GetUserIndex(Choice);
		if (index >= 0)
			App->GetCurrentUser()->Load(App->Users[index]);
		return LEVELCHOICE;
	}

	return LOADGAME;
}
