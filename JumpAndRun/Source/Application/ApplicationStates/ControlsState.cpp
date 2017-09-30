#include<allegro.h>
#include"ControlsState.h"

using namespace APPLICATION;
using namespace FRAMEWORK;

void ControlsState::OnKeyDown(int key)
{
	if (Blocked) return;

	if (WaitForKey)
	{
		Ctrls.Keys[Pos] = key;
		WaitForKey = false;
	}
	else
	{
		if (key == KEY_UP) Pos--;
		if (key == KEY_DOWN) Pos++;

		if (key == KEY_ENTER)
		{
			if (Pos >= 0 && Pos < 4)
				WaitForKey = true;

			Choice = Pos;
		}
	}

	Pos = modulo(Pos, 6);
	Blocked = true;
}

void ControlsState::OnKeyUp(int key)
{
	Blocked = false;
}

void ControlsState::OnDraw()
{
	Vec2 c = RendererObj->GetScreen().GetCenter();
	int x = c.x - 230;
	int y = c.y - 150;

	Rect r; r.x = x; r.w = 450; r.h = 50;

	for (int i = 0; i<4; i++)
	{
		r.y = y + 50 * i;
		RendererObj->DrawTransRect(r, Pos == i ? 0x00ff00 : 0x000000, 0.5f);
		if (WaitForKey && Choice == i)
			RendererObj->DrawString(Controls::KeyLabels[i] + ": ", x, y + 50 * i + 5);
		else
			RendererObj->DrawString(Controls::KeyLabels[i] + ": " + Keyboard::GetKeyName(Ctrls.Keys[i]), x, y + 50 * i + 5);

	}

	r.y = y + 250;
	RendererObj->DrawTransRect(r, Pos == 4 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("OK", x + 200, y + 255);

	r.y = y + 300;
	RendererObj->DrawTransRect(r, Pos == 5 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Anuluj", x + 170, y + 305);
}

ApplicationState::APPSTATE ControlsState::OnUpdate(float dt)
{
	if (Choice == OK)
	{
		pGame->SetControls(Ctrls);
		return MAINMENU;
	}
	if (Choice == CANCEL)
	{
		return MAINMENU;
	}

	return CONTROLS;
}