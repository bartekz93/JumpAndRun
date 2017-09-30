#pragma once
#include<string>
#include"../Renderer/Renderer.h"

namespace GAME
{

	class Background
	{
	private:
		RENDERER::GraphicsSettings* Settings;
	public:
		BITMAP* Back;
		float Offset;
		int Width;
		int Height;
	public:
		void Create(const std::string& file, RENDERER::GraphicsSettings& set);
		void Update(float velocity, float dt);
		void Release()
		{
			//destroy_bitmap(Back);
		}
	};

}