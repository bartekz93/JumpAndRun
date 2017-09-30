#include"Platform.h"

using namespace GAME;

void FloatingPlatform::OnCreate()
{
	InitPos = Pos;
	Static = false;
	Nonmovable = true;
	Gravitable = false;
	Dir = Vec2(1.0f, 0.0f);
	Elasticity = 1.0f;
}

void FloatingPlatform::OnUpdate(float dt)
{
	Velocity.y = 0.0f;
	Pos.y = InitPos.y;
	AddForce(Dir * 20);
}