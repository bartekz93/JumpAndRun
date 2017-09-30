#pragma once
#include<string>

namespace APPLICATION
{
	struct LevelDesc
	{
		std::string File;
		int Points;
		int Enemies;

		LevelDesc() : File(""), Points(0), Enemies(0) {}
	};

	struct LevelStats
	{
		int Points;
		int Enemies;
		float Time;

		LevelStats() : Points(0), Enemies(0), Time(0) {}
		LevelStats(int p, int e, int t) : Points(p), Enemies(e), Time(t) {}
	};
}
