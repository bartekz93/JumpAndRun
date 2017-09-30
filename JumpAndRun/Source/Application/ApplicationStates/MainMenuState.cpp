#include<allegro.h>
#include"../Application.h"
#include"MainMenuState.h"
#include"../../Renderer/SurfacesManager.h"

using namespace FRAMEWORK;
using namespace APPLICATION;
using namespace RENDERER;

void MainMenuState::OnInit()
{
	Logo.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\logo.bmp");
	Logo.FrameSize = Vec2(Logo.Surface->w, Logo.Surface->h);
	CalcLogoRect();
	GameObj.SetKeyboard(App->GetKeyboard());
	GameObj.SetMusic(App->Music);
	GameObj.Init("gfx\\menu.bmp", false);
	Menu.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\menufinal.bmp");

	Choice = -1;

	Pos = 0;
	NumPos = 5;
	Blocked = false;
	Menu.FrameSize = Vec2(202.0f, 47.25f);
}

void MainMenuState::OnKeyDown(int key)
{
	if (Blocked) return;

	if (key == KEY_UP) Pos--;
	if (key == KEY_DOWN) Pos++;

	if (key == KEY_ENTER)
	{
		Choice = Pos;
	}

	Pos = modulo(Pos, NumPos);
	Blocked = true;
}

void MainMenuState::OnKeyUp(int key)
{
	Blocked = false;
}

ApplicationState::APPSTATE MainMenuState::OnUpdate(float dt)
{
	switch ((MENUPOS)Choice)
	{
	case QUIT:
		return ApplicationState::QUIT;
		break;
	case NEWGAME:
		return ApplicationState::NEWGAME;
		break;
	case LOAD:
		return ApplicationState::LOADGAME;
		break;
	case HIGHSCORES:
		return ApplicationState::HIGHSCORES;
		break;
	case OPTIONS:
		return ApplicationState::OPTIONS;
	default:
		return ApplicationState::MAINMENU;
	}
}

void MainMenuState::OnRelease()
{
	GameObj.Release();
}

void MainMenuState::OnDraw()
{
	Vec2 center = RendererObj->GetScreen().GetCenter();

	Rect window = Rect(center.x - 150, center.y - 150, 300, 300);

	RendererObj->DrawTransRect(window, 0x000000, 0.2f);

	for (int i = 0; i<NumPos; i++)
	{
		Menu.FrameIndex.y = i;
		Menu.FrameIndex.x = 0;
		if (i == Pos) Menu.FrameIndex.x = 1;

		RendererObj->DrawSprite(Menu, center.x, center.y + Menu.FrameSize.y*i - 2 * Menu.FrameSize.y);
	}
}