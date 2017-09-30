#pragma once
#include<map>
#include<vector>

namespace RENDERER
{
	class SurfacesManager
	{
	private:
		static std::vector<BITMAP*> Bitmaps;
		static std::map<std::string, BITMAP*> Images;
	public:
		static BITMAP* CreateSurface(int w, int h);
		static BITMAP* CreateSurfaceFromBMP(const std::string& file);
		static void Release();
	
		static int GetPixel(BITMAP* surface, int x, int y);
	};
}