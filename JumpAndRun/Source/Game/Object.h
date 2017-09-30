#pragma once
#include"../Framework/Math.h"

namespace GAME
{

	class Object
	{
		friend class World;
	public:
		enum TYPE {
			NONE = 0, PLATFORM, PLAYER, BODY, STAR, ENEMY, FLPLATFORM, FALLPLATFORM, FINISH,
			STATICSTAR, BONUS, BOX, JUMPERL, JUMPERR, JUMPERU, JUMPERD, STARBUNCH
		};

		int Color;
		TYPE Type;

		Vec2 LastPos;
		Vec2 Pos;
		Vec2 NextPos;

		Rect AABB;
		Vec2 Velocity;
		Vec2 Forces;
		float Mass;
		float Elasticity;
		Vec2 TilePos;
		float Width;
		float Height;

		bool Nonmovable;
		bool Gravitable;
		bool Colideable;
		bool Live;
		bool Static;
		bool Visible;
		bool ScreenSpace;


		void AddForce(const Vec2& force) { Forces += force; }
		void Update(float dt);
		void Collide(Object& obj, const Vec2& pushVec);

		void SolveOverlap(const Vec2& pushVector, Object& obj, float dt);
		void SolveCollision(const Vec2& normalVec, Object& obj);

		virtual void OnCreate() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnCollision(Object& obj, const Vec2& normal) {}

		Object() : Static(true), Visible(true), Live(true), Colideable(true), Gravitable(true), Nonmovable(false), Elasticity(0.0f), ScreenSpace(false),
			Velocity(Vec2(0.0f, 0.0f)), Forces(Vec2(0.0f, 0.0f)) {}
		virtual ~Object() {}
	};

	class ObjectBunch : public Object
	{
	public:
		int Amount;
	};
};
using namespace GAME;