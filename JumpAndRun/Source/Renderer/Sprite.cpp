#include<allegro.h>
#include"../Framework/Math.h"
#include"Sprite.h"


using namespace RENDERER;

void AnimateSprite::NextFrame()
{
	int framesx = ((BITMAP*)Surface)->w / FrameSize.x;
	int framesy = ((BITMAP*)Surface)->h / FrameSize.y;

	if (Type == X)
	{	
		FrameIndex.x++;
		if (FrameIndex.x >= framesx)
		{
			FrameIndex.x = 0.0f;
		}
	}
	if (Type == Y)
	{	
		FrameIndex.y++;
		if (FrameIndex.y >= framesy)
		{
			FrameIndex.y = 0.0f;
		}
	}

	if (Type == XY)
	{	
		FrameIndex.x++;

		if (FrameIndex.x >= framesx)
		{
			FrameIndex.y++;
			FrameIndex.x = 0.0f;
			if (FrameIndex.y >= framesy)
			{
				FrameIndex.y = 0.0f;
			}
		}
	}
}

void AnimateSprite::Update(float dt)
{
	static float Time = 0.0f;
	Time += dt;

	if (Time >= FrameTime)
	{
		NextFrame();	
		Time = 0.0f;
	}
}