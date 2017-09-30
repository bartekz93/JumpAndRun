#pragma once
#include<time.h>
#include"Particles.h"
#include"../Framework/Messages.h"

using namespace FRAMEWORK;
using namespace RENDERER;


int super_rand()
{
  return (rand() << 15) | rand();
}

float fRandom(float beg, float end)
{
	UINT f = (end-beg)*100;
	if (!f) return beg;
	return (float)((super_rand()%f) + beg*100)/100;
}

Vec2 vRandom(Vec2 beg, Vec2 end)
{
	float x = fRandom(beg.x, end.x);
	float y = fRandom(beg.y, end.y);
	return Vec2(x, y);
}

int iRandom(int beg, int end)
{
	UINT i = end-beg;
	if (!i) return 0.0f;
	return (rand()%(i))+beg;
}

void ParticleEmitter::SetDesc(ParticleEmitterDesc& desc)
{
	srand(time(NULL));

	Desc = &desc;
	Particles = new Particle[desc.MaxParticles];

	for (UINT i=0;i<desc.MaxParticles;i++)
	{
		Particles[i].Age = 0.2f;
		Particles[i].Dir = desc.Dir;
	}
}

void ParticleEmitter::Clear()
{
	Particle* p;
	for (UINT i=0;i<Desc->MaxParticles;i++)
	{
		p = &Particles[i];
		p->Age = p->LifeTime;
	}
}

void ParticleEmitter::Update(float dt, bool c)
{
	dt *= Desc->SimulationSpeed;
	Particle* p;
	for (UINT i=0;i<Desc->MaxParticles;i++)
	{
		p = &Particles[i];

		float t = (p->Age / p->LifeTime);
		
		
		

		if (p->Age + dt <= p->LifeTime && p->Age != 0.0f)
		{
			//if (p->Dir != Vec2(0.0f, 0.0f)) p->Dir = p->Dir.Normalize();
			p->Color	= (Desc->Color	   -  Desc->Color0)	   *  t  +  Desc->Color0;
		p->Angle	= (Desc->Angle	   -  Desc->Angle0)	   *  t  +  Desc->Angle0;
		p->Alpha	= (Desc->Alpha	   -  Desc->Alpha0)	   *  t  +  Desc->Alpha0;

			p->Pos += p->Dir * p->Velocity * dt;
		}
		else if (c)
		{
			p->LifeCount ++;
			p->Dir = Desc->Dir != Vec2(0.0f, 0.0f) ? Desc->Dir.Normalize() : p->Dir;
			p->Pos		= vRandom(Vec2(Pos.x-Desc->Width/2, Pos.y-Desc->Height/2), 
								  Vec2(Pos.x+Desc->Width/2, Pos.y+Desc->Height/2)); 

			//p->Dir		= vRandom(Desc->Dir0, Desc->Dir);
			if (Desc->Spread)
			{
				float a =  ( fRandom(0.0f, Desc->Spread) - Desc->Spread*0.5 ) * (PI / 180.0f) + asinf(Desc->Dir.x);
				p->Dir.y = -cos(a); 
				p->Dir.x = sin(a);
			}
			
			p->LifeTime = fRandom(Desc->LifeTime0, Desc->LifeTime);
			p->Velocity = fRandom(Desc->Velocity0, Desc->Velocity);
			p->Alpha = Desc->Alpha0;
			p->Age = 0.2f;
		}
		

		//if (p->LifeCount > Desc->MaxLifeCount) continue;
		Particles[i].Age += dt;
	}
}

void ParticleEmitter::Release()
{
	if (Particles) delete[] Particles;
}