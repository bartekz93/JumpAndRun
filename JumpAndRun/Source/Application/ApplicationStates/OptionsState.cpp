#include<allegro.h>
#include<algorithm>
#include"../../Framework/Framework.h"
#include"../Application.h"
#include"OptionsState.h"

using namespace APPLICATION;
using namespace FRAMEWORK;

void OptionsState::OnKeyDown(int key)
{
	if (Blocked) return;

	if (key == KEY_UP) Pos--;
	if (key == KEY_DOWN) Pos++;

	if (key == KEY_LEFT)
	{
		if (Pos == MUSICVOLUME)
		{
			TempAudioSettings.MusicVolume = max(TempAudioSettings.MusicVolume - 0.01f, 0.0f);
			App->GetAudio()->SetSettings(TempAudioSettings);
		}
		if (Pos == SOUNDVOLUME)
		{
			TempAudioSettings.SoundVolume = max(TempAudioSettings.SoundVolume - 0.01f, 0.0f);
			App->GetAudio()->SetSettings(TempAudioSettings);
			TestSnd->reset();
			TestSnd->play();
		}
		if (Pos == FULLSCREEN)
		{
			TempSettings.Fullscreen = !TempSettings.Fullscreen;
		}
		if (Pos == RESOLUTION)
		{
			TempSettings.ResIndex = max(0, TempSettings.ResIndex - 1);
			TempSettings.Width = RendererObj->Resolutions[TempSettings.ResIndex].W;
			TempSettings.Height = RendererObj->Resolutions[TempSettings.ResIndex].H;
		}
	}
	else if (key == KEY_RIGHT)
	{
		if (Pos == MUSICVOLUME)
		{
			TempAudioSettings.MusicVolume = min(TempAudioSettings.MusicVolume + 0.01f, 1.0f);
			App->GetAudio()->SetSettings(TempAudioSettings);
		}
		if (Pos == SOUNDVOLUME)
		{
			TempAudioSettings.SoundVolume = min(TempAudioSettings.SoundVolume + 0.01f, 1.0f);
			App->GetAudio()->SetSettings(TempAudioSettings);
			TestSnd->reset();
			TestSnd->play();
		}
		if (Pos == FULLSCREEN)
		{
			TempSettings.Fullscreen = !TempSettings.Fullscreen;
		}
		if (Pos == RESOLUTION)
		{
			TempSettings.ResIndex = min((int)RendererObj->Resolutions.size() - 1, TempSettings.ResIndex + 1);
			TempSettings.Width = RendererObj->Resolutions[TempSettings.ResIndex].W;
			TempSettings.Height = RendererObj->Resolutions[TempSettings.ResIndex].H;
		}
	}


	if (key == KEY_ENTER)
	{
		Choice = Pos;
	}

	Pos = modulo(Pos, 7);
	Blocked = true;
}

void OptionsState::OnKeyUp(int key)
{
	Blocked = false;
}

ApplicationState::APPSTATE OptionsState::OnUpdate(float dt)
{

	if (Choice == CANCEL) return MAINMENU;

	if (Choice == OK)
	{

		//if (TempSettings.Fullscreen != Settings->Fullscreen || TempSettings.Width != Settings->Width || TempSettings.Height != Settings->Height)
		App->Reboot(TempSettings);
		CalcLogoRect();
		GameObj.OnChangeGraphicsMode();

		GameObj.OffsetVec = Vec2(RendererObj->GetScreen().w*0.5f - 400, RendererObj->GetScreen().h*0.5f - 300);
		//RendererObj->SetScreen(Rect(TempSettings.Width*0.5f-400, TempSettings.Width*0.5f-400, TempSettings.Width, TempSettings.Height)); 

		return MAINMENU;
	}

	if (Choice == CHANGECTRLS)
	{
		return CONTROLS;
	}


	return OPTIONS;
}


void OptionsState::OnDraw()
{
	Vec2 c = RendererObj->GetScreen().GetCenter();
	int x = c.x - 200;
	int y = c.y - 150;

	Rect r; r.x = x; r.w = 400; r.h = 50;

	r.y = y;
	RendererObj->DrawTransRect(r, Pos == 0 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Glosnosc muzyki: " + IntToStr(TempAudioSettings.MusicVolume * 100), x, y + 5);

	r.y = y + 50;
	RendererObj->DrawTransRect(r, Pos == 1 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Glosnosc dzwieku: " + IntToStr(TempAudioSettings.SoundVolume * 100), x, y + 55);

	r.y = y + 100;
	RendererObj->DrawTransRect(r, Pos == 2 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Pelny ekran: " + string(TempSettings.Fullscreen ? "TAK" : "NIE"), x, y + 105);

	r.y = y + 150;
	RendererObj->DrawTransRect(r, Pos == 3 ? 0x00ff00 : 0x000000, 0.5f);
	string res = IntToStr(RendererObj->Resolutions[TempSettings.ResIndex].W) + "x" + IntToStr(RendererObj->Resolutions[TempSettings.ResIndex].H);
	RendererObj->DrawString("Rozdzielczosc: " + res, x, y + 155);

	r.y = y + 200;
	RendererObj->DrawTransRect(r, Pos == 4 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Sterowanie >>", x, y + 205);

	r.y = y + 300;
	RendererObj->DrawTransRect(r, Pos == 5 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("OK", x + 180, y + 305);

	r.y = y + 350;
	RendererObj->DrawTransRect(r, Pos == 6 ? 0x00ff00 : 0x000000, 0.5f);
	RendererObj->DrawString("Anuluj", x + 160, y + 355);
}

void OptionsState::OnInit()
{
	Audio.LoadSound("sfx\\star.mp3", TestSnd);
}