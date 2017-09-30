#include<allegro.h>
#include"../Application.h"
#include"GameState.h"

using namespace APPLICATION;
using namespace FRAMEWORK;

ApplicationState::APPSTATE GameState::Update(float dt)
{
	GameObj.UpdateSender(dt);

	if (State == GAME)
	{
		GameObj.GetPlayer()->Listen();

		Game::RETURNCODE code = GameObj.Update(dt);

		if (code == Game::RETRYLVL)
		{
			State = GAMEOVER;
		}
		if (code == Game::NEXTLVL)
		{
			State = GAMESTATS;

			App->GetCurrentUser()->Stats[Level].Points = GameObj.GetPlayer()->GetPoints();
			App->GetCurrentUser()->Stats[Level].Enemies = 0;
			App->GetCurrentUser()->Stats[Level].Time = GameObj.GetTime();
			App->GetCurrentUser()->Save();

			Pos = 1;
		}
	}
	else if (State == MENU || State == GAMEOVER || State == GAMESTATS)
	{
		GameObj.GetPlayer()->Sleep();



		if (Choice == (int)GameState::MENU::MAINMENU)
		{
			App->GetCurrentUser()->Stats[Level].Points = GameObj.GetPlayer()->GetPoints();
			App->GetCurrentUser()->Stats[Level].Enemies = 0;
			App->GetCurrentUser()->Stats[Level].Time = GameObj.GetTime();
			App->GetCurrentUser()->Save();

			this->GameObj.Release();

			return ApplicationState::MAINMENU;
		}
	}

	return ApplicationState::GAME;
}

void GameState::Draw()
{
	GameObj.Draw();

	if (State != GAME)
	{
		Rect scr = RendererObj->GetScreen();
		RendererObj->DrawTransRect(scr, 0x000000, 0.2f);

		Vec2 c = RendererObj->GetScreen().GetCenter();

		if (State == GAMESTATS)
		{
			Rect r = Rect(c.x - 200, c.y - 120, 400, 200);
			RendererObj->DrawTransRect(r, 0x000000, 0.2f);

			RendererObj->DrawSurface(Game::StarBig, c.x - 180, c.y - 55 - 32);
			RendererObj->DrawString(IntToStr(GameObj.GetPlayer()->GetPoints()), c.x - 148, c.y - 100);

			RendererObj->DrawSurface(Game::Clock, c.x, c.y - 87);
			RendererObj->DrawString(IntToStr(GameObj.GetTime()), c.x + 52, c.y - 100);
		}

		for (int i = 0; i<2; i++)
		{
			Rect r = Rect(c.x - 150, c.y - 55 + i * 50, 300, 50);
			RendererObj->DrawTransRect(r, i == Pos ? 0x00ff00 : 0x000000, 0.5f);

			if (State == MENU)
				RendererObj->DrawString(MenuStr[i], r.x, r.y);
			else if (State == GAMEOVER)
				RendererObj->DrawString(GameOverStr[i], r.x, r.y);
			else if (State == GAMESTATS)
				RendererObj->DrawString(GameStatsStr[i], r.x, r.y);
		}
	}

	RendererObj->DrawString("FPS: " + IntToStr(App->GetFPS()), 10, RendererObj->GetScreen().h - 50);
}

void GameState::Init()
{
	MenuStr[0] = "Powrot do gry";
	MenuStr[1] = "Menu glowne";

	GameOverStr[0] = "Jeszcze raz";
	GameOverStr[1] = "Menu glowne";

	GameStatsStr[0] = "Nastepny poziom";
	GameStatsStr[1] = "Menu glowne";
}


void GameState::Release()
{
	//GameObj.Release();
}

void GameState::OnOpen()
{
	GameObj.SetKeyboard(App->GetKeyboard());
	Map = App->GetLevelDesc(Level).File;
	GameObj.Init(Map);
	GameObj.SetMaxPoints(App->GetLevelDesc(Level).Points);

	Pos = 0;
	Choice = -1;
	State = GAME;
}

void GameState::OnKeyDown(int key)
{
	if (GAME) return;
	if (Blocked) return;

	if (key == KEY_UP) Pos--;
	if (key == KEY_DOWN) Pos++;
	Pos = modulo(Pos, 2);

	if (key == KEY_ENTER)
	{
		Choice = Pos;
		if (Choice == (int)RETURN)
		{
			if (State == MENU)
				State = GAME;

			if (State == GAMEOVER)
			{
				//GameObj.Release();
				GameObj.RestartGame();

				Pos = 0;
				Choice = -1;
				State = GAME;
				//GameObj.Init(Map);
				//OnOpen();
			}

			if (State == GAMESTATS)
			{
				GameObj.Release();
				Level = modulo(Level + 1, 3);
				OnOpen();
			}
		}
	}


	Blocked = true;
}

void GameState::OnKeyUp(int key)
{
	if (GAME) return;
	Blocked = false;
}