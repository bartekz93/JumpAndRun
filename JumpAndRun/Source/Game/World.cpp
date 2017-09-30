#include<allegro.h>
#include<algorithm>
#include"../Renderer/SurfacesManager.h"
#include"../Game/Game.h"
#include"../Game/World.h"
#include"../Game/Jumper.h"
#include"../Game/Platform.h"
#include"../Game/Enemy.h"



using namespace MATH;
using namespace RENDERER;
using namespace std;

typedef unsigned int Uint32;

bool RectTest(const Rect& a, const Rect& b)
{
	if ((a.x > b.x && a.x < b.x+b.w) || (a.y > b.y && a.y < b.y+b.h) ||
	    (a.x+a.w > b.x && a.x+a.w < b.x+b.w) || (a.y+a.h > b.y && a.y+a.h < b.y+b.h))
		return true;
	return false;
}

bool World::CheckCollision(Object& obj1, Object& obj2, float dt)
{
	if (obj1.Static && obj2.Static) return false;

	float deltaX = abs(obj1.Pos.x - obj2.Pos.x);
	float deltaY = abs(obj1.Pos.y - obj2.Pos.y);

	

	float xOverlap = max((obj1.Width + obj2.Width)*0.5f - deltaX, 0.0f);
	float yOverlap = max((obj1.Height + obj2.Height)*0.5f - deltaY, 0.0f);

	Vec2 pushVec;
	bool yColl = false;

	if (xOverlap != 0.0f && yOverlap != 0.0f)
	{
	
		if (xOverlap < yOverlap)
		{
			pushVec = Vec2(obj1.Pos.x - obj2.Pos.x, 0.0f);
			pushVec = pushVec.Normalize();

			if (pGame->Collision(obj1, obj2, pushVec)) obj1.SolveOverlap(pushVec*xOverlap, obj2, dt);
			

			return true;
		}

		if (yOverlap < xOverlap)
		{
			pushVec = Vec2(0.0f,  obj1.Pos.y - obj2.Pos.y);
			pushVec = pushVec.Normalize();

			if (pGame->Collision(obj1, obj2, pushVec)) obj1.SolveOverlap(pushVec*yOverlap, obj2, dt);
			
			return true;
		}
	}

	return false;
}

Object::TYPE World::ColorToObjectType(int color)
{
	map<int, Object::TYPE>::iterator it =  Palette.find(color);
	if (it == Palette.end() || it->second == Object::NONE)
	{
		return Object::NONE;
	}
	else return it->second;
}

void WorldBuilder::Build(World& world, const string& path, const Vec2& OffsetVec)
{
	BITMAP* mapSurf = SurfacesManager::CreateSurfaceFromBMP(path);
	BITMAP* palette = SurfacesManager::CreateSurfaceFromBMP("gfx\\palette.bmp");
	WorldObj = &world;

	for (int i=0;i<palette->w;i++)
	{
		world.Palette[SurfacesManager::GetPixel(palette, i, 0)] = (Object::TYPE)i;
	}

	
	
	world.Width = mapSurf->w;
	world.Height = mapSurf->h;

	world.Map = new Object::TYPE*[world.Height];
	CheckedFields = new bool*[world.Height];
	for (int i=0;i<world.Height;i++)
	{
		world.Map[i] = new Object::TYPE[world.Width];
		CheckedFields[i] = new bool[world.Width];
	}

	for (int i=0;i<world.Height;i++)
	{
		for (int j=0;j<world.Width;j++)
		{
			CheckedFields[i][j] = false;

			Uint32 pixel = SurfacesManager::GetPixel(mapSurf, j, i);
			world.Map[i][j] = world.ColorToObjectType(pixel);
		}
	}

	Object* pl = NULL;
	int plIndex = 0;

	for (int i=0;i<world.Height;i++)
	{
		for (int j=0;j<world.Width;j++)
		{
			if (!CheckedFields[i][j])
			{
				Object::TYPE objT = world.Map[i][j];
				
				if (objT != Object::NONE)
				{
					world.Objects.push_back(BuildObject(objT, j, i, OffsetVec));

					if (objT == Object::PLAYER)
					{
						plIndex = world.Objects.size()-1;
						pl = world.Objects[plIndex];
					}
				}
				else
				{
					CheckedFields[i][j] = true;
				}
			}
		}
	}

	if (world.Objects.size() > 0 && pl)
	{
		Object* tmp = world.Objects[0];
		world.Objects[0] = pl;
		world.Objects[plIndex] = tmp;
	}

	for (int i=0;i<world.Height;i++)
		delete[] CheckedFields[i];
	delete[] CheckedFields;
}

Object* WorldBuilder::BuildObject(Object::TYPE objT, int x, int y, const Vec2& OffsetVec)
{
	Object::TYPE fieldT;
	int widthT = 0;
	int widthB = 0;
	int heightR = 0;
	int heightL = 0;

	int i = 1;
	fieldT = WorldObj->Map[y][x];
	while (fieldT == objT)
	{
		fieldT = WorldObj->Map[y][x+i];
		widthT++;
		i++;
	}

	i = 1;
	fieldT = WorldObj->Map[y][x];
	while (fieldT == objT)
	{
		fieldT = WorldObj->Map[y+i][x];
		heightL++;
		i++;
	}

	i = 1;
	fieldT = WorldObj->Map[y+heightL-1][x];
	while (fieldT == objT)
	{
		fieldT = WorldObj->Map[y+heightL-1][x+i];
		widthB++;
		i++;
	}

	int width = min(widthT, widthB);

	i = 1;
	fieldT = WorldObj->Map[y][x+width-1];
	while (fieldT == objT)
	{
		fieldT = WorldObj->Map[y+i][x+width-1];
		heightR++;
		i++;
	}

	int height = min(heightL, heightR);

	for (int j=0;j<height;j++)
	{
		for (int k=0;k<width;k++)
		{
			CheckedFields[j+y][k+x] = true;
		}
	}

	Object* obj;

	


	if (objT == Object::PLAYER)
		obj = new Player;
	else if (objT == Object::FLPLATFORM)
		obj = new FloatingPlatform;
	else if (objT == Object::ENEMY)
	{
		obj = new Enemy;
		((Enemy*)obj)->SetWorld(WorldObj);
	}
	else if (objT == Object::STAR)
	{
		obj = new Object;
		obj->Static = false;
		obj->Elasticity = 1.0f;
		obj->Velocity = Vec2(0.0f, 0.0f);
	}
	else if (objT == Object::BONUS)
	{
		obj = new Bonus;
		obj->Static = false;
		obj->Elasticity = 1.0f;
	}
	else if (objT == Object::STATICSTAR)
	{
		obj = new Object;
		obj->Static = true;
		obj->Velocity = Vec2(0.0f, 0.0f);
	}
	else if (objT == Object::BOX)
	{
		obj = new Object;
		obj->Static = false;
		obj->Type = Object::BOX;
	}
	else if (objT == Object::JUMPERL) obj = new Jumper(Jumper::LEFT);
	else if (objT == Object::JUMPERR) obj = new Jumper(Jumper::RIGHT);
	else if (objT == Object::JUMPERU) obj = new Jumper(Jumper::UP);
	else if (objT == Object::JUMPERD) obj = new Jumper(Jumper::DOWN);
	
	else obj = new Object;
	
	obj->Color = 0xff0000ff;
	obj->Pos = Vec2((x+width*0.5f)*WorldObj->TileSize+OffsetVec.x, (y+height*0.5f)*WorldObj->TileSize+OffsetVec.y);
	obj->Width = width*WorldObj->TileSize;
	obj->Height = height*WorldObj->TileSize;
	obj->Mass = 2.0f;
	obj->Type = objT;
	if (objT >= Object::JUMPERL && objT <= Object::JUMPERD)obj->Height = 5.0f;		

	obj->OnCreate();

	return obj;
}

Object::TYPE World::GetObjectType(const Vec2& pos) const
{
	if (pos.x < Width && pos.y < Height && pos.x >= 0 && pos.y >= 0)
		return Map[(int)pos.y][(int)pos.x];
	return Object::NONE;
}


void World::Update(float dt)
{
	//_sleep(10);
	float minleft = PlayerObj->Pos.x - Screen.w*0.5f;
	float mintop =  PlayerObj->Pos.y - Screen.h*0.5f;
	float maxleft = Width*TileSize - Screen.w;
	float maxtop =  Height*TileSize - Screen.h;

	Screen.x = min(max(minleft, 0.0f), maxleft);
	Screen.y = min(max(mintop, 0.0f),  maxtop);


	for (int i=0;i<Objects.size();i++)
	{
		if (Objects[i]->Static || !Objects[i]->Live) continue;

		Objects[i]->Visible = RectTest(Objects[i]->AABB, Screen);

		if (Objects[i]->Gravitable) Objects[i]->AddForce(Gravity);
		Objects[i]->Update(dt);
		if (Objects[i]->Colideable)
		{
			for (int j=0;j<Objects.size();j++)
			{
				if (i != j && Objects[j]->Live && Objects[j]->Colideable)
					CheckCollision(*Objects[i], *Objects[j], dt);
			}
		}
	}
}

void World::Create(const string& map, const Vec2& gravity, const Vec2& OffsetVec, int tilesize)
{
	Gravity = gravity;
	TileSize = tilesize;

	WorldBuilder Builder;
	Builder.Build(*this, map, OffsetVec);

	PlayerObj = (Player*)Objects[0];
	PlayerObj->Static = false;	
}

void World::Relase()
{
	for (int i=0;i<Objects.size();i++)
	{
		delete Objects[i];
	}
	Objects.clear();

	for (int i=0;i<Height;i++)
		delete[] Map[i];
	delete[] Map;
}