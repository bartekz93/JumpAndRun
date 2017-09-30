#pragma once
#include"Object.h"

namespace GAME
{

	class Jumper : public Object
	{
	public:
		enum JUMPERTYPE { LEFT, RIGHT, UP, DOWN };
		JUMPERTYPE JumperType;
		int Power;

		void Jump(Object* obj)
		{
			switch (JumperType)
			{
			case LEFT: 
				obj->Velocity.x = Power * 2; 
				break;
			case RIGHT: 
				obj->Velocity.x = -Power * 2; 
				break;
			case UP: 
				obj->Velocity.y = -Power; 
				break;
			case DOWN: 
				obj->Velocity.y = Power; 
				break;
			};
		}

		Jumper(JUMPERTYPE jt) : Power(80), JumperType(jt) {}
	};

}