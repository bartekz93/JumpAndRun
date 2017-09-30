#pragma once
#include"Object.h"
#include"Movable.h"

namespace GAME
{

	class Platform : public Object
	{
		enum TYPE { SOLID, BUMP };
	};


	class FloatingPlatform : public Platform, public Movable
	{
		Vec2 InitPos;
		void OnCreate();
		void FloatingPlatform::OnUpdate(float dt);
	};

}