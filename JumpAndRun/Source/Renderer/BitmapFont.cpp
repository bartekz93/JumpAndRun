#include<allegro.h>
#include<fstream>
#include"BitmapFont.h"
#include"SurfacesManager.h"

using namespace RENDERER;
using namespace std;

void BitmapFont::Load(const string& name)
{
	fstream file;
	file.open(name + ".csv", ios::in);

	file >> XCount;
	file >> YCount;

	file >> CellWidth;
	file >> CellHeight;

	file >> Size;

	BaseWidth = new int[Size];

	for (int i = 0; i<Size; i++)
	{
		file >> BaseWidth[i];
	}

	file.close();

	Bitmap = SurfacesManager::CreateSurfaceFromBMP(name + ".bmp");
}