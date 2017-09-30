#pragma once

namespace RENDERER
{

	struct GraphicsSettings
	{
		int Width;
		int Height;
		int ColorDepth;
		bool Fullscreen;
		int SoundVolume;
		int MusicVolume;
		int ResIndex;

		GraphicsSettings(int w, int h, int c, bool f = false) : Width(w), Height(h), ColorDepth(c), Fullscreen(f), SoundVolume(5), MusicVolume(5), ResIndex(1) {}
		GraphicsSettings() {}
	};

	class Resolution
	{
	public:
		int W;
		int H;

		Resolution(int w, int h) : W(w), H(h) {}
	};

}