#pragma once
#include"../Framework/Math.h"

namespace GAME
{
	class Movable
	{
	protected:
		MATH::Vec2 Dir;
	public:
		MATH::Vec2 GetDir()
		{
			return Dir;
		}

		void SetDir(const MATH::Vec2& v)
		{
			Dir = v;
		}

		void InvertDir()
		{
			Dir = -Dir;
		}

		virtual ~Movable() {}
	};
}