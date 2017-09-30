#include"../Game/Background.h"
#include"../Renderer/SurfacesManager.h"

using namespace std;
using namespace RENDERER;

void Background::Create(const string& file, GraphicsSettings& set)
{
	Back = SurfacesManager::CreateSurfaceFromBMP(file);

	Settings = &set;

	Offset = 0;
}

void Background::Update(float velocity, float dt)
{
	Offset -= velocity*dt * 5;

	if (Offset <= -Settings->Width) Offset = Settings->Width;
	if (Offset >= Settings->Width) Offset = 0;

	Width = Settings->Width;
	Height = Settings->Height;
}