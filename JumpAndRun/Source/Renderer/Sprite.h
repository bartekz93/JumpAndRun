#pragma once
#include<allegro.h>
#include"../Framework/Math.h"

namespace RENDERER
{
	class Sprite
	{
	public:
		BITMAP* Surface;
		MATH::Vec2 FrameSize;
		MATH::Vec2 FrameIndex;
	
		Sprite() {}
	
	};
	
	
	class AnimateSprite : public Sprite
	{
	public:
		enum TYPE { X, Y, XY };
		TYPE Type;
		float FrameTime;
	
		AnimateSprite() : Type(X), FrameTime(1.0f) {}
	
		void NextFrame();
		void Update(float dt);
	};

};