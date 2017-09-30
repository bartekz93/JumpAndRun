#include"Object.h"

void Object::Update(float dt)
{
	if (!Live) return;

	AABB.x = Pos.x - Width*0.5f;
	AABB.y = Pos.y - Height*0.5f;
	AABB.w = Width;
	AABB.h = Height;

	if (Static) return;

	Vec2 Acceleration = Forces / Mass;
	Velocity += Acceleration*dt;
	LastPos = Pos;
	Pos += Velocity*dt;
	Velocity.x = Velocity.x * 0.78f;

	

	TilePos = (Pos / 15).ToInt();

	Forces = Vec2(0.0f, 0.0f);

	OnUpdate(dt);
	//Velocity = Vec2(0.0f, 0.0f);
}


void Object::SolveOverlap(const Vec2& pushVec, Object& obj, float dt)
{
	if (Static && obj.Static) return;

	if (Static) 
	{
		obj.Pos -= pushVec;
	}
	else if (obj.Static) 
	{
		Pos += pushVec;
	}
	else 
	{
		Pos += pushVec*0.5f;
		obj.Pos -= pushVec*0.5f;	
	}

	Vec2 Normal = pushVec.Normalize();
	SolveCollision(Normal, obj);
}

void Object::SolveCollision(const Vec2& normal, Object& obj)
{
	//obj.Static = false;
	Vec2 V = Velocity - obj.Velocity;

	Vec2 Vn, Vt;
	
	Vn = normal * V.Dot(normal);
	Vt = V - Vn;

	if (V.Dot(normal) > 0.0f) Vn = Vec2(0.0f, 0.0f);

	float friction = 0.2f;

	if (Vt.Dot(Vt) < 0.01f) friction = 1.01f;

	V = Vn * -(1 + Elasticity);// - Vt * (friction);

	float M = 1.0f / Mass + 1.0f / obj.Mass;

	float r0 = (1.0f / Mass) / M;
	float r1 = (1.0f / obj.Mass) / M;


	if (Static) obj.Velocity -= V* r1 - V * r0;
	else if (obj.Static) Velocity += V * r0 + V* r1;
	else
	{
		Velocity += V * r0;
		obj.Velocity -= V* r1;
	}

	OnCollision(obj, normal);
}