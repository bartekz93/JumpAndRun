#pragma once
#include"Object.h"
#include"Movable.h"

namespace GAME
{

	class Enemy : public Object, public Movable
	{
	private:
		const class World* pWorld;
		Vec2 LastReturn;
	public:
		void OnUpdate(float dt);
		void OnCreate();
		void OnCollision(Object& obj, const Vec2& normal) {}

		void SetWorld(const World* w) { pWorld = w; }
	};

}