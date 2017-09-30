#include<allegro.h>
#include<fstream>
#include<sstream>
#include<algorithm>
#include"Game.h"
#include"Jumper.h"
#include"Enemy.h"
#include"Platform.h"
#include"ObjectPair.h"
#include"../Renderer/SurfacesManager.h"


using namespace APPLICATION;
using namespace FRAMEWORK;
using namespace RENDERER;


Renderer*		RendererUser::RendererObj = NULL;
string			Controls::KeyLabels[4];
string			Bonus::Names[NUMBONUS] = { "Bieg", "Rozmycie", "Pancerz", "Brak swiatla", "Gwiazdki x2", "Ciagly ruch" };
Controls		Game::DefaultControls = Controls(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
BITMAP*			Game::StarBig;
BITMAP*			Game::HeartBig;
BITMAP*			Game::Clock;
OutputStreamPtr Game::Music;



void Game::OnChangeGraphicsMode()
{
	destroy_bitmap(Buffer);
	destroy_bitmap(Mask);
	Buffer = create_bitmap(RendererObj->GetScreen().w, RendererObj->GetScreen().h);
	Mask = create_bitmap(RendererObj->GetScreen().w, RendererObj->GetScreen().h);
}

void Game::RestartGame()
{
	KeyboardObj->RemoveListener(WorldObj.PlayerObj);
	WorldObj.Relase();
	WorldObj.Create(MapFile, Vec2(0.0f, 10.0f), Vec2(0.0f, 0.0f), 15);
	WorldObj.pGame = this;

	if (WorldObj.PlayerObj->Type == Object::PLAYER)
	{
		KeyboardObj->AddListener(*WorldObj.PlayerObj);
		WorldObj.PlayerObj->SetControls(UserControls);
	}
	
	Time = 0.0f;
	End = false;
	ReturnCode = CONTINUE;
	Music->setPitchShift(1.0f);

	WorldObj.PlayerObj->Clear();
	Particles.Clear();
	GraphicsSender::Clear();
}

void Game::Init(const string& map, bool hud, const Vec2& OffsetVector)
{
	OffsetVec = OffsetVector;
	HUD = hud;
	Time = 0.0f;
	End = false;
	ReturnCode = CONTINUE;
	MapFile = map;

	FlowTimer.Create(Delegate<Game>(this, &Game::FlowFunction));
	FlowTimer.Start(Time, 0.05f);

	WorldObj.Screen = Rect(0, 0, RendererObj->GetScreen().w, RendererObj->GetScreen().h);

	WorldObj.Create(map, Vec2(0.0f, 10.0f), OffsetVector, 15);
	WorldObj.pGame = this;

	int offX = 10;
	int offY = 10;
	int sX = 100;
	int sY = 50;
	PointsField = Rect(offX, offY, sX, sY);
	LivesField = Rect(RendererObj->GetScreen().w-offX-sX, offY, sX, sY);

	if (WorldObj.PlayerObj->Type == Object::PLAYER)
	{
		KeyboardObj->AddListener(*WorldObj.PlayerObj);
		WorldObj.PlayerObj->SetControls(UserControls);
	}


	Finish = SurfacesManager::CreateSurfaceFromBMP("gfx\\finish.bmp");
	BodySprite = SurfacesManager::CreateSurfaceFromBMP("gfx\\body.bmp");
	Star = SurfacesManager::CreateSurfaceFromBMP("gfx\\star.bmp");
	StarBig = SurfacesManager::CreateSurfaceFromBMP("gfx\\starbig.bmp");
	HeartBig = SurfacesManager::CreateSurfaceFromBMP("gfx\\heartbig.bmp");
	Clock = SurfacesManager::CreateSurfaceFromBMP("gfx\\clockbig.bmp");
	BonusSprite = SurfacesManager::CreateSurfaceFromBMP("gfx\\bonus.bmp");
	BoxSprite = SurfacesManager::CreateSurfaceFromBMP("gfx\\box.bmp");

	Audio.LoadSound("sfx\\star.mp3", StarSound);
	Audio.LoadSound("sfx\\scream.mp3", ScreamSound);
	Audio.LoadSound("sfx\\bonus.mp3", BonusSound);
	Audio.LoadSound("sfx\\turtle.mp3", TurtleSound);
	Audio.LoadSound("sfx\\jump.mp3", JumpSound);
	Audio.UpdateVolume();
	//StarSound;
	//ScramSound
	//BonusSound
	//TurtleSoun
	//JumpSound;
	
	Buffer = create_bitmap(RendererObj->GetScreen().w, RendererObj->GetScreen().h);
	Mask = create_bitmap(RendererObj->GetScreen().w, RendererObj->GetScreen().h);

	Backimage.Create("gfx\\back.bmp", RendererObj->GetSettings());

	JumperSprite.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\jumper.bmp");
	JumperSprite.FrameSize = Vec2(32, 32);


	Tileset[0].Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\tiles.bmp");
	Tileset[0].FrameSize = Vec2(15, 15);
	Tileset[1].Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\tilesfall.bmp");
	Tileset[1].FrameSize = Vec2(15, 15);

	Particle.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\particles.bmp");
	Particle.FrameSize = Vec2(32, 32);
	Particle.FrameIndex = Vec2(1, 1);

	ParticlesDesc.Width = 10;
	ParticlesDesc.Height = 10;
	ParticlesDesc.Depth = 1.0f;

	ParticlesDesc.Angle = 0; ParticlesDesc.Angle0 = 0;
	//ParticlesDesc.Dir0 = Vec3(-0.2f, -1.0f, 0.0f);  ParticlesDesc.Dir = Vec3(0.2f, -1.0f, 0.0f);
	ParticlesDesc.Dir = Vec2(0, -1); 
	ParticlesDesc.Spread = 0;
	ParticlesDesc.Color0 = Vec3(0.1, 0.07, 0.0)*255; ParticlesDesc.Color = Vec3(0.09, 0.06, 0.06)*255;
	ParticlesDesc.MaxParticles = 500;
	ParticlesDesc.LifeTime = 20;    ParticlesDesc.LifeTime0 = 1;
	ParticlesDesc.Velocity = 50;  ParticlesDesc.Velocity0 = 30;
	ParticlesDesc.SpriteObj = &Particle;
	ParticlesDesc.SimulationSpeed = 3.0f;
	ParticlesDesc.Alpha0 = 255.0f;			ParticlesDesc.Alpha = 0.0f;
	ParticlesDesc.MaxLifeCount = 3;

	Particles.SetDesc(ParticlesDesc);

	anim.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\pies.bmp");
	anim.FrameSize = Vec2(45, 45);
	anim.FrameTime = 0.08f;

	EnemyAnim.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\zolw.bmp");
	EnemyAnim.FrameSize = Vec2(45, 45);
	EnemyAnim.FrameTime = 0.08f;

	//BonusAnim.Surface = SurfacesManager::CreateSurfaceFromBMP("gfx\\bonus.bmp");
	//BonusAnim.FrameSize = Vec2(25, 25);
	//BonusAnim.FrameTime = 0.08f;

	srand(time(0));
	//Particles.Pos = Vec2(400, 300);
}

void Game::Draw()
{
	//_sleep(300);

	RendererObj->SetDrawTarget(Buffer);

	RendererObj->DrawSurface(Backimage.Back, Backimage.Offset+Backimage.Width*0.5f, Backimage.Height*0.5f, Backimage.Width, Backimage.Height);
	if (Backimage.Offset > 0)
	{
		RendererObj->DrawSurface(Backimage.Back, Backimage.Offset - Backimage.Width + Backimage.Width*0.5f, Backimage.Height*0.5f, Backimage.Width, Backimage.Height);
	}
	else
	{
		RendererObj->DrawSurface(Backimage.Back, Backimage.Offset + Backimage.Width + Backimage.Width*0.5f, Backimage.Height*0.5f, Backimage.Width, Backimage.Height);
	}

	//Rect r = RendererObj->GetScreen();
	//RendererObj->DrawTransRect(r, 0x000000, Time);

	for (int i=0;i<Objects.size();i++)
	{
		ObjectBunch* obj = (ObjectBunch*)Objects[i].pObj;
		if (obj->Type == Object::STARBUNCH)
		{
			RendererObj->DrawSurface(Star, obj->Pos.x+OffsetVec.x, obj->Pos.y+OffsetVec.y);
			RendererObj->DrawString(IntToStr(obj->Amount), obj->Pos.x+OffsetVec.x+16, obj->Pos.y+OffsetVec.y);
		}
	}

	for (int i=0;i<WorldObj.Objects.size();i++)
	{
		Object* obj = WorldObj.Objects[i];

		if (!obj->Visible) continue;

		if (obj->Type >= Object::JUMPERL && obj->Type <= Object::JUMPERD)
		{
			Jumper* j = (Jumper*)obj;
			JumperSprite.FrameIndex = Vec2((int)j->JumperType, 0);
			RendererObj->DrawSprite(JumperSprite, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
		}

		if (obj->Type == Object::BOX)
		{
			RendererObj->DrawSurface(BoxSprite, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
		}

		if (obj->Type == Object::FINISH)
		{
			RendererObj->DrawSurface(Finish, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
		}

		if (obj->Type == Object::BONUS)
		{
			RendererObj->DrawSurface(BonusSprite, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
			//RendererObj->DrawSprite(BonusAnim, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
		}

		if (obj->Type == Object::STAR || obj->Type == Object::STATICSTAR)
		{
			if (!obj->ScreenSpace)
				RendererObj->DrawSurface(Star, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
			else
				RendererObj->DrawSurface(Star, obj->Pos.x+OffsetVec.x, obj->Pos.y+OffsetVec.y);
		}
		if (obj->Type == Object::PLATFORM || obj->Type == Object::FLPLATFORM || obj->Type == Object::FALLPLATFORM )
		{
			int index = obj->Type == Object::FALLPLATFORM ? 1 : 0;
			//SDL_SetAlpha(PlatformSprite, SDL_SRCALPHA, 70);

			//RendererObj.DrawSurface(PlatformSprite, obj->Pos.x-Screen.x, obj->Pos.y-Screen.y);
			//RendererObj.DrawFillRect(obj->Pos.x-WorldObj.Screen.x, obj->Pos.y-WorldObj.Screen.y, obj->Width, obj->Height, obj->Color);

			int tileW = obj->Width/WorldObj.TileSize;
			int tileH = obj->Height/WorldObj.TileSize;
			int tilex, tiley;

			for (int k=0;k<tileH;k++)
			{
				for (int j=0;j<tileW;j++)
				{
					if (k == 0) tiley = 0;
					else if (k == 1) tiley = 1;
					else if (k == tileH-1) tiley = 3;
					else tiley = 2;

					if (j == 0) tilex = 0;
					else if (j == tileW-1) tilex = 2;
					else tilex = 1;

					Tileset[index].FrameIndex = Vec2(tilex, tiley);

					RendererObj->DrawSprite(Tileset[index], WorldObj.TileSize*j + obj->Pos.x - obj->Width*0.5f + WorldObj.TileSize*0.5f - WorldObj.Screen.x+OffsetVec.x, 
						WorldObj.TileSize*k + obj->Pos.y - obj->Height*0.5f + WorldObj.TileSize*0.5f - WorldObj.Screen.y+OffsetVec.y);
				}
			}
		}

		if (obj->Type == Object::PLAYER)
		{
			Player* pl = (Player*)obj;

			if (pl->GetDir().x == 0.0f)
				anim.FrameIndex.y = pl->Velocity.x > 0 ? 0 : 1;
			else
				anim.FrameIndex.y = pl->GetDir().x > 0 ? 0 : 1;

			if (WorldObj.PlayerObj->CurrentBonus.BonusType == Bonus::RUN)
				RendererObj->DrawParticles(Particles, WorldObj.Screen.x, WorldObj.Screen.y);

			if (pl->Armor && ((int)(Time*10.0f) - (int)Time*10) % 2 == 0)
			{
			}
			else
			{
				RendererObj->DrawSprite(anim, obj->Pos.x-WorldObj.Screen.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);
			}
		}
		if (obj->Type == Object::BODY)
		{
			RendererObj->DrawSurface(BodySprite, obj->Pos.x-WorldObj.Screen.x, obj->Pos.y-WorldObj.Screen.y);
		}
	}

	for (int i=0;i<WorldObj.Objects.size();i++)
	{
		Object* obj = WorldObj.Objects[i];

		if (obj->Type == Object::ENEMY)
		{
			EnemyAnim.FrameIndex.y = ((Enemy*)obj)->GetDir().x == 1 ? 0 : 1;
			RendererObj->DrawSprite(EnemyAnim, obj->Pos.x-WorldObj.Screen.x+OffsetVec.x, obj->Pos.y-WorldObj.Screen.y+OffsetVec.y);

		}
	}

	if (WorldObj.PlayerObj->Type == Object::PLAYER &&  WorldObj.PlayerObj->GetLives() < 0)
	{
		Rect r = RendererObj->GetScreen();
		RendererObj->DrawTransRect(r, 0xff0000, 0.3f);
	}

	if (WorldObj.PlayerObj->CurrentBonus.BonusType == Bonus::BLUR)
	{
		RendererObj->DrawTransSprite(Buffer, Flow, Flow, 0.5f);
		RendererObj->DrawTransSprite(Buffer, -Flow, -Flow, 0.5f);
	}

	RendererObj->SetDrawTarget(NULL);
	RendererObj->DrawFullscreenBitmap(Buffer);

	Vec2 v = WorldObj.PlayerObj->Velocity;
	Vec2 p = WorldObj.PlayerObj->Pos - Vec2(WorldObj.Screen.x, WorldObj.Screen.y);

	//RendererObj->DrawTransRect(RendererObj->GetScreen(), 0x0000ff, 0.5f);

	if (WorldObj.PlayerObj->CurrentBonus.BonusType == Bonus::LIGHTSOUT)
	{
		clear(Mask);
		circlefill(Mask, p.x, p.y, 150, 0xff00ff);
		RendererObj->DrawFullscreenBitmap(Mask);
		RendererObj->DrawTransSprite(Buffer, 0, 0, 0.05f);
	}


	if (HUD)
	{
		//RendererObj->DrawRect(PointsField, 0xff0000);
		RendererObj->DrawSurface(StarBig, PointsField.GetCenter().x, PointsField.GetCenter().y);

		//RendererObj->DrawRect(LivesField, 0xff0000);
		RendererObj->DrawSurface(HeartBig, LivesField.GetCenter().x, LivesField.GetCenter().y);
		//RendererObj.DrawString(IntToStr(WorldObj.PlayerObj->GetPoints()), PointsField.GetCenter().x, PointsField.GetCenter().y);

		RendererObj->DrawString(IntToStr(WorldObj.PlayerObj->GetPoints())/*+"/"+IntToStr(MaxPoints)*/, PointsField.GetCenter().x+32, PointsField.GetCenter().y-16);
		//RendererObj->DrawNumber(PointsField.GetCenter().x+32, PointsField.GetCenter().y-16, WorldObj.PlayerObj->GetPoints());
		//RendererObj->DrawNumber(LivesField.GetCenter().x+32, LivesField.GetCenter().y-16, WorldObj.PlayerObj->GetLives());
		RendererObj->DrawString(IntToStr(WorldObj.PlayerObj->GetLives()), LivesField.GetCenter().x-32, LivesField.GetCenter().y-16);

		RendererObj->DrawSurface(Clock, PointsField.GetCenter().x, PointsField.GetCenter().y+47);
		string time = FloatToStr(Time);
		RendererObj->DrawString(string(time, 0, time.find_first_of('.')+2), PointsField.GetCenter().x+32, PointsField.GetCenter().y+32);


		if (WorldObj.PlayerObj->CurrentBonus.BonusType != Bonus::NONE)
		{
			
			int k = 11 - (Time - WorldObj.PlayerObj->CurrentBonus.StartTime);
			if (k > 0)
			{
				RendererObj->DrawFillRect(Rect(RendererObj->GetScreen().w*0.5f-125, 10, 250, 40), 0x000000);
				RendererObj->DrawFillRect(Rect(RendererObj->GetScreen().w*0.5f-124, 11, k*25-2, 40-2), 0xff0000);
				RendererObj->DrawString(Bonus::Names[WorldObj.PlayerObj->CurrentBonus.BonusType-1], RendererObj->GetScreen().w*0.5f-125, 10);
			}
			else
			{
				EndBonus();
				WorldObj.PlayerObj->BonusEnd();
			}
		}

		if (WorldObj.PlayerObj->bCombo)
		{
			int c = 4 - (Time - WorldObj.PlayerObj->ComboTime);
			if (c > 0)
			{
				RendererObj->DrawFillRect(Rect(RendererObj->GetScreen().w*0.5f-125, RendererObj->GetScreen().h-50, 250, 40), 0x000000);
				RendererObj->DrawFillRect(Rect(RendererObj->GetScreen().w*0.5f-124, RendererObj->GetScreen().h-49, c*83-2, 40-2), (WorldObj.PlayerObj->Combo*20)<<8);
				RendererObj->DrawString("Combo: "+IntToStr(WorldObj.PlayerObj->Combo), RendererObj->GetScreen().w*0.5f-125, RendererObj->GetScreen().h-50);
			}
			else
			{
				WorldObj.PlayerObj->ComboEnd();
			}
		}
	}
}

Bonus::BONUSTYPE Game::RandBonus()
{
	//return Bonus::PERMMOVE;
	return (Bonus::BONUSTYPE)(rand()%(Bonus::NUMBONUS-1)+1);
}

void Game::FlowFunction()
{
	Flow += flowDir;

	if (Flow >= 10) flowDir = -1;
	if (Flow <= -10) flowDir = 1;
}

Game::RETURNCODE Game::Update(float dt)
{
	if (End)
	{
		return ReturnCode;
	}

	//_sleep(80);
	static float TimeStep = 1.0f / 60.0f;
	static float Acumulator = 0.0f;
	//static int MaxSteps = 10;

	Acumulator += dt;
	int steps = (int)(Acumulator / TimeStep);
	if (dt > 1.0f) 
	{
		Acumulator = 0.0f;
		return CONTINUE;
	}

	for (int i=0;i<steps;i++)
	{
		Acumulator -= TimeStep;
		Time += TimeStep;

		WorldObj.Update(TimeStep*12);

		EnemyAnim.Update(TimeStep);
		//BonusAnim.Update(TimeStep);

		if (WorldObj.PlayerObj->CurrentBonus.BonusType != Bonus::ARMOR && 
			WorldObj.PlayerObj->Armor && Time > WorldObj.PlayerObj->ArmorTime + 4) WorldObj.PlayerObj->Armor = false;

		if (WorldObj.PlayerObj->Pos.y-WorldObj.Screen.y > WorldObj.Screen.h)
		{
			return Game::RETRYLVL;
		}

		if (WorldObj.PlayerObj->GetDir().x != 0.0f) anim.Update(TimeStep);
		else
		{
			anim.FrameIndex = Vec2(0.0f, 0.0f);
		}
	}

	
	Backimage.Update(WorldObj.PlayerObj->Velocity.x, dt);
	
	
	Particles.Pos = Vec2(WorldObj.PlayerObj->Pos.x, WorldObj.PlayerObj->Pos.y);
	Particles.Update(dt, WorldObj.PlayerObj->GetDir().x != 0.0f && WorldObj.PlayerObj->CurrentBonus.BonusType == Bonus::RUN);
	
	FlowTimer.Update(dt);
	
	return CONTINUE;
	
	
	//WorldObj.Update(dt*5);
}




void Game::Release()
{
	KeyboardObj->RemoveListener(WorldObj.PlayerObj);
	GraphicsSender::Clear();
	
	Backimage.Release();
	Particles.Release();
	WorldObj.Relase();
}

bool Game::Collision(Object& obj1, Object& obj2, const Vec2& normal)
{
	ObjectPair pair(obj1, obj2);

	if (pair.Is(Object::PLAYER, Object::JUMPERL) || 
		pair.Is(Object::PLAYER, Object::JUMPERR) ||
		pair.Is(Object::PLAYER, Object::JUMPERU) ||
		pair.Is(Object::PLAYER, Object::JUMPERD))
	{
		JumpSound->reset();
		JumpSound->play();

		((Jumper*)pair.GetObject(2))->Jump(pair.GetObject(Object::PLAYER));
		((Player*)pair.GetObject(Object::PLAYER))->SetState(Player::JUMP2);
		return false;
	}

	if (pair.Is(Object::BOX, Object::JUMPERL) || 
		pair.Is(Object::BOX, Object::JUMPERR) ||
		pair.Is(Object::BOX, Object::JUMPERU) ||
		pair.Is(Object::BOX, Object::JUMPERD))
	{
		JumpSound->reset();
		JumpSound->play();

		((Jumper*)pair.GetObject(2))->Jump(pair.GetObject(Object::BOX));
		return false;
	}

	if (pair.Is(Object::PLAYER, Object::STAR) )
	{
		StarSound->reset();
		StarSound->play();
		pair.GetObject(Object::STAR)->Live = false;
		

		pair.GetObject(Object::STAR)->Pos -= Vec2(WorldObj.Screen.x, WorldObj.Screen.y);
		pair.GetObject(Object::STAR)->ScreenSpace = true;

		Send(*pair.GetObject(Object::STAR), PointsField, 400);

		return false;
	}

	if (pair.Is(Object::PLAYER, Object::BONUS) )
	{
		BonusSound->reset();
		BonusSound->play();

		Player* pl = ((Player*)pair.GetObject(Object::PLAYER));
		pair.GetObject(Object::BONUS)->Live = false;
		pair.GetObject(Object::BONUS)->Visible = false;

		Bonus::BONUSTYPE bt = RandBonus();
		pl->SetBonus(bt, Time);
		StartBonus();
		
		
		return false;
	}

	if (pair.Is(Object::PLAYER, Object::STATICSTAR) )
	{
		StarSound->reset();
		StarSound->play();

		pair.GetObject(Object::STATICSTAR)->Live = false;

		pair.GetObject(Object::STATICSTAR)->Pos -= Vec2(WorldObj.Screen.x, WorldObj.Screen.y);
		pair.GetObject(Object::STATICSTAR)->ScreenSpace = true;

		Send(*pair.GetObject(Object::STATICSTAR), PointsField, 400);

		return false;
	}

	if (pair.Is(Object::PLAYER, Object::FINISH))
	{
		Object* f = pair.GetObject(Object::FINISH);
		Player* pl = (Player*)pair.GetObject(Object::PLAYER);

		
			ReturnCode = Game::RETURNCODE::NEXTLVL;
			End = true;
		

		return false;
	}

	if (pair.Is(Object::ENEMY, Object::ENEMY)) return false;
	if (pair.Is(Object::BOX, Object::STAR) ) return false;

	if (pair.Is(Object::BOX, Object::PLAYER))
	{
		/*Player* pl = (Player*)pair.GetObject(Object::PLAYER);
		Object* box = pair.GetObject(Object::BOX);

		if (pl->GetState() == Player::STAMP)
		{
			ObjectBunch* stars = new ObjectBunch;
			stars->Type = Object::STARBUNCH;
			stars->Pos = box->Pos - Vec2(WorldObj.Screen.x, WorldObj.Screen.y);
			stars->ScreenSpace = true;
			stars->Amount = 10;

			Send(*stars, PointsField, 400);
		}*/
	}

	if (pair.Is(Object::BOX, Object::STATICSTAR) ) return false;

	if (pair.Is(Object::ENEMY, Object::PLAYER))
	{
		

		Player* pl = (Player*)pair.GetObject(Object::PLAYER);
		Enemy* en = (Enemy*)pair.GetObject(Object::ENEMY);

		//if ()
		if (pl->TilePos.y == en->TilePos.y-2.0f)
		//if (GetNeighbour(pl, 6) == Object::ENEMY)
		{
			TurtleSound->reset();
			TurtleSound->play();

			pl->ComboTime = Time;
			pl->Combo++;

			ObjectBunch* stars = new ObjectBunch;
			stars->Type = Object::STARBUNCH;
			stars->Pos = en->Pos - Vec2(WorldObj.Screen.x, WorldObj.Screen.y);
			stars->ScreenSpace = true;
			stars->Amount = pl->Combo*10;

			Send(*stars, PointsField, 400);

			pl->bCombo = true;
			pl->Velocity.y = -0.4f*pl->Velocity.y;
			en->Colideable = false;

		}
		else if (pl->TilePos == en->TilePos)
		{
			if (!pl->Armor)
			{
				ScreamSound->reset();
				ScreamSound->play();
			}
			if (!pl->Dead(Time))
			{
				pl->Colideable = false;
				pl->AddForce(Vec2(0.0f, -100.0f));
				pl->Armor = true;
				//End = true;
				//ReturnCode = Game::RETRYLVL;
			}
		}
		

		return false;
	}

	if (pair.Is(Object::ENEMY, Object::STAR))
	{
		return false;
	}
	if (pair.Is(Object::STAR, Object::STAR))
	{
		return false;
	}

	if (pair.Is(Object::FLPLATFORM, Object::STAR))
	{
		return false;
	}
	if (pair.Is(Object::FLPLATFORM, Object::STATICSTAR))
	{
		return false;
	}

	//if (pair.Is(Object::PLAYER, Object::FLPLATFORM)) return false;

	if (pair.Is(Object::PLATFORM, Object::FLPLATFORM) || pair.Is(Object::FLPLATFORM, Object::FALLPLATFORM))
	{
		((FloatingPlatform*)pair.GetObject(Object::FLPLATFORM))->InvertDir();
	}

	if (pair.Is(Object::PLAYER, Object::PLATFORM))
	{
		Player* pl = ((Player*)pair.GetObject(Object::PLAYER));
		if (GetNeighbour(pl, 6) == Object::PLATFORM)
		{
			pl->SetState(Player::STAND);
		}
		
		//pair.GetObject(Object::PLATFORM)->Static = false;
	}

	if (pair.Is(Object::PLAYER, Object::FLPLATFORM))
	{
		((Player*)pair.GetObject(Object::PLAYER))->Forces += ((FloatingPlatform*)pair.GetObject(Object::FLPLATFORM))->Forces*0.5f;
		((Player*)pair.GetObject(Object::PLAYER))->SetState(Player::STAND);
		
		//pair.GetObject(Object::PLATFORM)->Static = false;
	}

	if (pair.Is(Object::PLAYER, Object::FALLPLATFORM))
	{
		if (((Player*)pair.GetObject(Object::PLAYER))->Pos.y < pair.GetObject(Object::FALLPLATFORM)->Pos.y)
		{
			((Player*)pair.GetObject(Object::PLAYER))->SetState(Player::STAND);
			pair.GetObject(Object::FALLPLATFORM)->Static = false;
			pair.GetObject(Object::FALLPLATFORM)->Colideable = false;
		}
	}

	if (pair.Is(Object::FALLPLATFORM, Object::FALLPLATFORM))
	{
		pair.GetObject(0)->Static = false;
		pair.GetObject(0)->Colideable = false;
		pair.GetObject(1)->Static = false;
		pair.GetObject(1)->Colideable = false;
	}


	return true;

}

Object::TYPE Game::GetNeighbour(Object* obj, int n)
{
	int x[] = { -1.0f, -0.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 1.0f };
	int y[] = { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	return WorldObj.GetObjectType(obj->TilePos+Vec2(x[n], y[n]));
}

void Game::OnReceive(Object& obj)
{
	if (obj.Type == Object::STAR || obj.Type == Object::STATICSTAR)
	{
		if (WorldObj.PlayerObj->CurrentBonus.BonusType == Bonus::DOUBLEPOINTS)
			WorldObj.PlayerObj->AddPoints(2);
		else
			WorldObj.PlayerObj->AddPoints();

		obj.Visible = false;
	}

	if (obj.Type == Object::STARBUNCH)
	{
		WorldObj.PlayerObj->AddPoints(((ObjectBunch*)&obj)->Amount);

		delete &obj;
	}
}

void GraphicsSender::Send(Object& obj, const Rect& rect, int speed)
{
	SendObjectDesc sobj;
	sobj.pObj = &obj;
	sobj.Dest = &rect;
	sobj.Dir = (rect.GetCenter() - obj.Pos).Normalize();
	sobj.Speed = speed;

	Objects.push_back(sobj);
}

void GraphicsSender::Clear()
{
	Objects.clear();
}

void GraphicsSender::UpdateSender(float dt)
{
	for (int i=0;i<Objects.size();i++)
	{
		SendObjectDesc* s = &Objects[i];
		Object* obj = s->pObj;

		obj->Pos = obj->Pos + s->Dir * dt * s->Speed;

		Vec2 v = (s->Dest->GetCenter() - obj->Pos).Normalize();

		float cos = (v.x*s->Dir.x + v.y * s->Dir.y) / 
			(sqrt(pow(v.x, 2) + pow(v.y, 2)) * sqrt(pow(s->Dir.x, 2) + pow(s->Dir.y, 2)));

		//if (RectPointTest(*s->Receiver->Dest, s->Pos) || (int)cos == -1) 
		if ((int)cos == -1)
		{
			OnReceive(*obj);
			Objects.erase(Objects.begin() + i ); 
		}
	}
}


void Game::StartBonus()
{
	Bonus::BONUSTYPE bt = WorldObj.PlayerObj->CurrentBonus.BonusType;

	if (bt == Bonus::RUN)
	{
		Music->setPitchShift(1.5f);
	}
	if (bt == Bonus::BLUR)
	{
		Music->setPitchShift(0.5f);
	}
}

void Game::EndBonus()
{
	Bonus::BONUSTYPE bt = WorldObj.PlayerObj->CurrentBonus.BonusType;

	if (bt == Bonus::RUN || bt == Bonus::BLUR)
	{
		Music->setPitchShift(1.0f);
	}
}