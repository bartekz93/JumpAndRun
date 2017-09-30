#pragma once
#include<string>

namespace GAME
{

	struct Controls
	{
		enum KEYS { LEFT, RIGHT, UP, DOWN };
		int Keys[4];
		static std::string KeyLabels[4];

		Controls(int l, int r, int u, int s)
		{
			Keys[0] = l; Keys[1] = r; Keys[2] = u; Keys[3] = s;
		}
		Controls() {}
	};

}