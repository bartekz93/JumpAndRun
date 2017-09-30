#include<allegro.h>
#include"../Framework/Messages.h"
#include"SurfacesManager.h"

using namespace FRAMEWORK;
using namespace RENDERER;

vector<BITMAP*> SurfacesManager::Bitmaps;
map<string, BITMAP*> SurfacesManager::Images;

BITMAP* SurfacesManager::CreateSurface(int w, int h)
{
	BITMAP* btmp = create_bitmap(w, h);

	if (!btmp) 
	{
		ERROR("B³¹d podczas tworzenia powierzchni");	
	}
	else
	{
		INFO("Utworzono powierzchniê");
		Bitmaps.push_back(btmp);
		return btmp;
	}
}


BITMAP* SurfacesManager::CreateSurfaceFromBMP(const string& file)
{
	map<string, BITMAP*>::iterator it = Images.find(file);
	if (it != Images.end()) return it->second;

	BITMAP* bmp = load_bitmap(file.c_str(), default_palette);
	if (!bmp)
	{
		WARNING("B³¹d podczas tworzenia powierzchni z "+file);
	}
	else
	{
		INFO("Utworzono powierzchniê z "+file);
		Images[file] = bmp;
		return bmp;
	}	
}

void SurfacesManager::Release()
{
	for (int i=0;i<Bitmaps.size();i++)
	{
		destroy_bitmap(Bitmaps[i]);
	}

	for (map<string, BITMAP*>::iterator it=Images.begin();it!=Images.end();it++)
	{
		destroy_bitmap(it->second);
	}
}

int SurfacesManager::GetPixel(BITMAP* surface, int x, int y)
{
	return getpixel(surface, x, y);
}