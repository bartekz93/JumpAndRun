#pragma once
#include<allegro.h>
#include<string>
#include<vector>
#include"../Framework/Math.h"
#include"../Renderer/Particles.h"
#include"../Renderer/Sprite.h"
#include"../Renderer/GraphicsSettings.h"
#include"../Renderer/BitmapFont.h"

namespace RENDERER
{
	class Renderer
	{
	private:
		BitmapFont Font;

		BITMAP* Buffer;	//render target
		BITMAP* RealBuffer;
		BITMAP* Numbers;

		GraphicsSettings Settings;
		MATH::Rect Screen;

		static GraphicsSettings DefaultSettings;
	public:
		std::vector<Resolution> Resolutions;

		Renderer();

		void SetScreen(const Rect& s) { Screen = s; }
		void SetFont(BitmapFont& font);
		void Init(const GraphicsSettings& settings);
		void ChangeGraphicsMode(GraphicsSettings& settings);
		void Release();

		void DrawRotateSprite(BITMAP* bmp, int x, int y, int a);
		void SetDrawTarget(BITMAP* target) { if (target) Buffer = target; else Buffer = RealBuffer; }
		void DrawTransSprite(BITMAP* bmp, int x, int y, float trans);
		void DrawFullscreenBitmap(BITMAP* bmp);
		void DrawNumber(int x, int y, int value);
		void DrawParticles(const ParticleEmitter& emitter, int x, int y);
		void DrawString(const std::string& str, int x, int y);
		void DrawSprite(const Sprite& sprite, int x, int y);
		void DrawSurface(BITMAP* surf, int x, int y);
		void DrawSurface(BITMAP* surf, int x, int y, int w, int h);
		void DrawFillRect(int x, int y, int w, int h, unsigned int color);
		void DrawFillRect(const MATH::Rect& r, UINT color);
		void DrawRect(const MATH::Rect& r, UINT color);
		void DrawTransRect(const MATH::Rect& r, UINT color, float trans);

		const MATH::Rect& GetScreen() const { return Screen; }
		GraphicsSettings& GetSettings() { return Settings; }

		void Begin();
		void End();
	};
}