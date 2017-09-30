#pragma once
#include<vector>
#include<map>
#include"../Framework/Math.h"
#include"../Game/Object.h"
#include"../Game/Player.h"

namespace GAME
{

	class World
	{
	friend  class Game;
	friend  class WorldBuilder;
	public:
		void Create(const std::string& map, const MATH::Vec2& gravity, const MATH::Vec2& OffsetVec, int tilesize = 15);
		bool CheckCollision(Object& b1, Object& b2, float dt);
		bool Collision(Object& obj1, Object& obj2);
	
		Object::TYPE ColorToObjectType(int color);
		Object::TYPE GetObjectType(const Vec2& pos) const;
		void Update(float dt);
		void Relase();
	private:
		Game* pGame;
	
		MATH::Vec2 Gravity;
		MATH::Rect Screen;
	
		int TileSize;
		int Width;
		int Height;
	
		std::map<int, Object::TYPE> Palette;
		Object::TYPE** Map;
		std::vector<Object*> Objects;
		Player* PlayerObj;
	};
	
	class WorldBuilder
	{
	private:
		bool** CheckedFields;
		World* WorldObj;
	
		Object* BuildObject(Object::TYPE type, int x, int y, const MATH::Vec2& OffsetVec);
	public:
		void Build(World& world, const std::string& path, const MATH::Vec2& OffsetVec);
	};

};

using namespace GAME;

