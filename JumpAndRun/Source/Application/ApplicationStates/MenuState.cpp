#include"MenuState.h"

using namespace APPLICATION;
using namespace RENDERER;
using namespace MATH;

Game MenuState::GameObj; //dla t³a z gry
Rect MenuState::LogoRect;
Sprite MenuState::Logo;

void MenuState::Init()
{
	OnInit();
}


void MenuState::CalcLogoRect()
{
	Vec2 center = RendererObj->GetScreen().GetCenter();
	LogoRect = Rect(center.x - Logo.Surface->w*0.5f, 100 - Logo.Surface->h*0.5f, Logo.Surface->w, Logo.Surface->h);
}

void MenuState::Draw()
{
	GameObj.Draw();
	Vec2 center = RendererObj->GetScreen().GetCenter();
	RendererObj->DrawSprite(Logo, center.x, 100.0f);
	//RendererObj->DrawRect(LogoRect, 0xff0000);

	OnDraw();
}


ApplicationState::APPSTATE MenuState::Update(float dt)
{
	GameObj.Update(dt);

	return OnUpdate(dt);

}