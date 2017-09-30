#include"User.h"
#include<fstream>

using namespace APPLICATION;
using namespace std;

void User::Save(bool fileExist)
{
	User us;
	us.Load(Name);

	fstream file;
	file.open("gfx\\" + Name + ".txt", ios::out);

	//file << Stats.size();
	//file << endl;
	if (fileExist)
	{
		for (int i = 0; i<Stats.size(); i++)
		{
			if (us.Stats[i].Points <= Stats[i].Points)
			{
				file << Stats[i].Points;
				file << endl;

				file << Stats[i].Enemies;
				file << endl;

				file << (int)Stats[i].Time;
				file << endl;
			}
			else
			{
				file << us.Stats[i].Points;
				file << endl;

				file << us.Stats[i].Enemies;
				file << endl;

				file << (int)us.Stats[i].Time;
				file << endl;
			}
		}
	}
	else
	{
		for (int i = 0; i<Stats.size(); i++)
		{
			file << Stats[i].Points;
			file << endl;
			file << Stats[i].Enemies;
			file << endl;
			file << (int)Stats[i].Time;
		}
	}

	file.close();
}

void User::Load(const string& name)
{
	fstream file;
	Name = name;
	file.open("gfx\\" + name + ".txt", ios::in);

	Stats.clear();

	for (int i = 0; i<3; i++)
	{
		int p, e, t;
		file >> p;
		file >> e;
		file >> t;

		Stats.push_back(LevelStats(p, e, t));
	}

	file.close();
}