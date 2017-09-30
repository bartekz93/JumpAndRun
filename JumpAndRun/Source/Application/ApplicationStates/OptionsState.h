#pragma once
#include"MenuState.h"

namespace APPLICATION
{
	class OptionsState : public MenuState, public AudioUser
	{
	private:
		int Pos;
		int Choice;
		bool Blocked;
		OutputStreamPtr TestSnd;

		RENDERER::GraphicsSettings* Settings;
		RENDERER::GraphicsSettings TempSettings;
		AudioSettings TempAudioSettings;

		enum ELEMENTS { MUSICVOLUME, SOUNDVOLUME, FULLSCREEN, RESOLUTION, CHANGECTRLS, OK, CANCEL };
	public:
		OptionsState() : MenuState(ApplicationState::OPTIONS) {}

		void Create(RENDERER::GraphicsSettings* settings) { Settings = settings; }
		APPSTATE OnQuit() { return ApplicationState::MAINMENU; }

		void OnKeyDown(int key);
		void OnKeyUp(int key);

		void OnOpen() { Choice = -1; Pos = 0; TempSettings = *Settings; TempAudioSettings = App->GetAudio()->GetSettings(); }
		void OnDraw();
		APPSTATE OnUpdate(float dt);
		void OnInit();
		void OnRelease() {}
	};
}
