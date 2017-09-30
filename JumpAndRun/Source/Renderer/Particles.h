#pragma once
#include"../Framework/Math.h"
#include"Sprite.h"


namespace RENDERER
{

struct ParticleEmitterDesc
{
public:
	MATH::Vec2  Dir; 
	MATH::Vec3  Color0, Color;
	float Spread;
	float Angle0,	 Angle;
	float Velocity0, Velocity;
	float LifeTime0, LifeTime;
	float Alpha0, Alpha;	
	UINT MaxLifeCount;

	Sprite* SpriteObj;
	UINT  MaxParticles;
	float Width, Height, Depth;
	float SimulationSpeed;

	ParticleEmitterDesc() :  SimulationSpeed(1.0f) {}
};

class Particle
{
	public:
	MATH::Vec2 Pos;
	MATH::Vec2 Dir;
	MATH::Vec3 Color;
	float Alpha;
	float Angle;
	float Velocity;
	float Age;
	float LifeTime;
	float LifeCount;

	Particle() : Age(0.0f), LifeCount(0) {}

};

class ParticleEmitter
{
private:
	ParticleEmitterDesc* Desc;
	Particle* Particles;
public:
	ParticleEmitter() : Particles(NULL) {}

	MATH::Vec2 Pos;

	void SetDesc(ParticleEmitterDesc& desc);

	ParticleEmitterDesc* GetDesc() const { return Desc; }
	const Particle* GetParticle(UINT i) const { return &Particles[i]; }

	void Clear();
	void Update(float dt, bool c);
	void Release();
};

};