#include"Enemy.h"
#include"World.h"

using namespace GAME;

void Enemy::OnCreate()
{
	Static = false;
	LastReturn = Vec2(-1, -1);
	Dir = Vec2(1.0f, 0.0f);
}

void Enemy::OnUpdate(float dt)
{
	AddForce(Dir * 10);
	Vec2 p = TilePos + Vec2(Dir.x, 2.0f);
	if (p != LastReturn && pWorld->GetObjectType(p) == Object::TYPE::NONE && pWorld->GetObjectType(TilePos + Vec2(0.0f, 2.0f)) == Object::PLATFORM)
	{
		LastReturn = p;
		Dir = -Dir;
	}
}