#pragma once
#include"Object.h"
#include<string>

namespace GAME
{

	class Bonus : public Object
	{
	public:
		enum BONUSTYPE { NONE, RUN, BLUR, ARMOR, LIGHTSOUT, DOUBLEPOINTS, PERMMOVE, NUMBONUS };
		static std::string Names[NUMBONUS];
	};

	struct BonusDesc
	{
		Bonus::BONUSTYPE BonusType;
		float StartTime;

		BonusDesc() : BonusType(Bonus::NONE) {}
		BonusDesc(Bonus::BONUSTYPE type, float st) : BonusType(type), StartTime(st) {}
	};

}
