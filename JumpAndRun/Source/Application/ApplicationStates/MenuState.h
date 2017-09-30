#pragma once
#include"../ApplicationState.h"
#include"../../Game/Game.h"
#include"../../Framework/Math.h"
#include"../../Renderer/Sprite.h"

namespace APPLICATION
{
	class MenuState : public ApplicationState
	{
	protected:
		static GAME::Game		GameObj; //dla t³a z gry
		static RENDERER::Sprite Logo;
		static MATH::Rect		LogoRect;
	public:
		MenuState(ApplicationState::APPSTATE appstate) : ApplicationState(appstate) {}

		void Init();
		void Draw();
		void Release() { OnRelease(); }
		void CalcLogoRect();

		APPSTATE Update(float dt);

		virtual void OnKeyDown(int key) {}
		virtual void OnKeyUp(int key) {}
		virtual void OnOpen() {}
		virtual void OnDraw() {}
		virtual void OnInit() {}
		virtual void OnRelease() {}

		virtual APPSTATE OnQuit()			{ return MAINMENU; }
		virtual APPSTATE OnUpdate(float dt) { return MAINMENU; }
	};
}