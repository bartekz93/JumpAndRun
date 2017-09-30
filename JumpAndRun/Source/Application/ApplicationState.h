#pragma once
#include"../Renderer/RendererUser.h"
#include"../Framework/Framework.h"

namespace APPLICATION
{

	class ApplicationState : public RENDERER::RendererUser, public FRAMEWORK::KeyboardListener
	{
		friend class Application;
	public:
		enum APPSTATE { MAINMENU, NEWGAME, LEVELCHOICE, HIGHSCORES, GAME, LOADGAME, OPTIONS, CONTROLS, NUMSTATES, QUIT };
	protected:
		static Application* App;


		APPSTATE AppState;
	public:

		ApplicationState() {}
		ApplicationState(APPSTATE state) : AppState(state) {}

		virtual APPSTATE OnQuit() = 0;
		virtual void Init() = 0;
		virtual void OnOpen() {}
		virtual APPSTATE Update(float dt) = 0;
		virtual void Draw() = 0;
		virtual void Release() = 0;
		virtual ~ApplicationState() {}

		virtual void OnKeyDown(int key) {}
		virtual void OnKeyUp(int key) {}
		virtual void OnChar(char c) {}
	};
}
