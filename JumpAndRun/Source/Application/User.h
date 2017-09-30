#pragma once
#include<string>
#include<vector>

#include"Level.h"

namespace APPLICATION
{
	class User
	{
	public:
		std::string Name;
		std::vector<LevelStats> Stats;
		//void Add

		void Save(bool fileExist = true);
		void Load(const std::string& name);
	};
}
