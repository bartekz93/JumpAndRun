#include<allegro.h>
#include"SurfacesManager.h"
#include"Sprite.h"
#include"Particles.h"
#include"Renderer.h"

using namespace RENDERER;
using namespace std;


GraphicsSettings Renderer::DefaultSettings;

UINT RgbToHex(Vec3 c)
{
	return (UINT)(255) | (UINT)c.x<<16 | (UINT)c.y<<8 | (UINT)c.z;
}

Renderer::Renderer()
{
	Resolutions.clear();
	Resolutions.push_back(Resolution(640, 480));
	Resolutions.push_back(Resolution(800, 600));
	Resolutions.push_back(Resolution(1024, 768));
	Resolutions.push_back(Resolution(1280, 720));
	Resolutions.push_back(Resolution(1280, 768));
	Resolutions.push_back(Resolution(1360, 768));
	Resolutions.push_back(Resolution(1366, 768));
	Resolutions.push_back(Resolution(1680, 1050));

	DefaultSettings = GraphicsSettings(800, 600, 32, false);
	Settings = DefaultSettings;
}

void Renderer::DrawParticles(const ParticleEmitter& emitter, int x, int y)
{
	const ParticleEmitterDesc* d = emitter.GetDesc();

	for (UINT i=0;i<d->MaxParticles;i++)
	{
		const Particle* p = emitter.GetParticle(i);
		//d->Sprite->SetColor(emitter->GetParticle(i)->Color);
		//d->Sprite->SetColor(p->Color.x, p->Color.y, p->Color.z, p->Alpha);
		//SDL_SetAlpha(d->SpriteObj->Surface, SDL_SRCALPHA, p->Alpha); 

		//DrawSprite(*d->SpriteObj, p->Pos.x - x, p->Pos.y - y);
		set_trans_blender(255,255,255, p->Alpha); 	
		static BITMAP* tmp = SurfacesManager::CreateSurface(32, 32);
		Sprite& sprite = *d->SpriteObj;
		blit(sprite.Surface, tmp, sprite.FrameIndex.x*sprite.FrameSize.x, sprite.FrameIndex.y*sprite.FrameSize.y, 0, 0, 
			sprite.FrameSize.x, sprite.FrameSize.y);

		draw_trans_sprite(Buffer, tmp,  p->Pos.x - x, p->Pos.y - y);
	}
}


void Renderer::DrawSprite(const Sprite& sprite, int x, int y)
{
	masked_blit(sprite.Surface, Buffer, sprite.FrameIndex.x*sprite.FrameSize.x, sprite.FrameIndex.y*sprite.FrameSize.y, x - sprite.FrameSize.x/2, y - sprite.FrameSize.y/2,sprite.FrameSize.x, sprite.FrameSize.y);
}

void Renderer::DrawTransRect(const Rect& r, UINT color, float trans)
{
	set_trans_blender(0, 0, 0, 256*trans);
	BITMAP* bmp = create_bitmap(r.w, r.h);
	rectfill(bmp, 0, 0, r.w, r.h, color);
	draw_trans_sprite(Buffer, bmp, r.x, r.y);
	destroy_bitmap(bmp);
}

void Renderer::ChangeGraphicsMode(GraphicsSettings& settings)
{
	GraphicsSettings tmp = Settings;
	Settings = settings;
	
	if (set_gfx_mode(Settings.Fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, Settings.Width, Settings.Height, 0, 0) != 0)
	{
		allegro_message("Ten tryb nie jest obslugiwany :/");
		Settings = tmp;
		set_gfx_mode(Settings.Fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, Settings.Width, Settings.Height, 0, 0);

		destroy_bitmap(RealBuffer);
		RealBuffer = create_bitmap(Settings.Width, Settings.Height);
		Buffer = RealBuffer;
	}

	destroy_bitmap(RealBuffer);
	RealBuffer = create_bitmap(Settings.Width, Settings.Height);
	Buffer = RealBuffer;
}

void Renderer::Init(const GraphicsSettings& settings)
{
	GraphicsSettings tmp = Settings;
	Settings = settings;

	

    allegro_init();
	install_keyboard();
	install_mouse();	

	set_color_depth(Settings.ColorDepth);
	if (set_gfx_mode(Settings.Fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, Settings.Width, Settings.Height, 0, 0) != 0)
	{
		allegro_message("Ten tryb nie jest obslugiwany :/");
		Settings = tmp;
		set_gfx_mode(Settings.Fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, Settings.Width, Settings.Height, 0, 0);
	}
	//show_os_cursor(1);

	RealBuffer = create_bitmap(Settings.Width, Settings.Height);
	Buffer = RealBuffer;
	Screen.x = 0.0f;
	Screen.y = 0.0f;
	Screen.w = Settings.Width;
	Screen.h = Settings.Height;

	Numbers = SurfacesManager::CreateSurfaceFromBMP("gfx\\numbers.bmp");
	

	Font.Load("gfx\\comic");
}

void Renderer::DrawString(const string& str, int x, int y )
{
	//textprintf_ex(Buffer, Font, x, y, 0xff000000, -1, str.c_str());

	int dx = x; 
	int dy = y;
	for (int i=0;i<str.size();i++)
	{
		int ch = str[i] - 32;
		int sy = ch / Font.XCount;
		int sx = ch - sy*Font.XCount;


		masked_blit(Font.Bitmap, Buffer, sx*Font.CellWidth, sy*Font.CellHeight, dx, dy, Font.CellWidth, Font.CellHeight);

		dx += Font.BaseWidth[ch];
	}
}

void Renderer::Release()
{
	Font.Release();
	destroy_bitmap(RealBuffer);
	allegro_exit();
}

void Renderer::Begin()
{
	clear_to_color(Buffer, 0xff000000);
}

void Renderer::End()
{
	blit(Buffer, screen, 0, 0, 0, 0, Settings.Width, Settings.Height); 
}

void Renderer::DrawRotateSprite(BITMAP* bmp, int x, int y, int a)
{
	rotate_sprite(Buffer, bmp, x, y, a);
}

void Renderer::DrawFillRect(int x, int y, int w, int h, unsigned int color)
{
	rectfill(Buffer, x, y, x+w, y+h, color);
}

void Renderer::DrawFillRect(const Rect& r, UINT color)
{
	rectfill(Buffer, r.x, r.y, r.x+r.w, r.y+r.h, color);
}



void Renderer::DrawNumber(int x, int y, int value)
{
	int n[3] = {0, 0, 0};
	n[0] = value / 100;
	n[1] = (value - n[0]*100) / 10;
	n[2] = (value - n[0]*100 - n[1]*10);

	int NumberW = 30;
	int NumberH = 31;

	int k = (n[0] ? 0 : (n[1] ? 1 : 2)); 
	for (int i=k;i<3;i++)
	{
		/*if (i == k && sign)
		{
			masked_blit( Numbers, Buffer, value >= 0 ? 10*NumberW : 11*NumberW, n[i] >=0 ? 0 : 24, x, y, NumberW, NumberH );
			x+= 20;
		}*/
  
		masked_blit( Numbers, Buffer, abs(n[i])*NumberW, n[i] >=0 ? 0 : 24, x, y, NumberW, NumberH );   
		
		 
		x += 20;
	}
}

void Renderer::DrawRect(const Rect& r, UINT color)
{
	rect(Buffer, r.x, r.y, r.x+r.w, r.y+r.h, color);
}

void Renderer::DrawSurface(BITMAP* sprite, int x, int y, int w, int h)
{
	stretch_blit(sprite, Buffer, 0, 0, sprite->w, sprite->h, x-w*0.5f, y-h*0.5f, w, h);
}

void Renderer::DrawSurface(BITMAP* sprite, int x, int y)
{
	masked_blit(sprite, Buffer, 0, 0, x-sprite->w*0.5f, y-sprite->h*0.5f, sprite->w, sprite->h);
};

void Renderer::DrawTransSprite(BITMAP* bmp, int x, int y, float trans)
{
	set_trans_blender(0, 0, 0, 256*trans);
	draw_trans_sprite(Buffer, bmp, x, y);
}

void Renderer::DrawFullscreenBitmap(BITMAP* bmp)
{
	masked_blit(bmp, Buffer, 0, 0, 0, 0, Settings.Width, Settings.Height);
}
