#pragma once
#include<string>
#include<vector>
#include<map>
#include"../Framework/Framework.h"
#include"../Framework/Math.h"
#include"../Framework/Audio.h"
#include"../Framework/Timer.h"
#include"../Renderer/RendererUser.h"
#include"../Renderer/Sprite.h"
#include"../Renderer/Particles.h"
#include"../Game/Background.h"
#include"../Game/World.h"
#include"../Game/Player.h"
#include"../Game/Controls.h"


namespace GAME
{






struct SendObjectDesc
{
	Object* pObj;
	const Rect* Dest;
	MATH::Vec2 Dir;
	int Speed;
};

class GraphicsSender
{
protected:
	std::vector<SendObjectDesc> Objects;
public:
	void Send(Object& obj, const Rect& rect, int speed);
	void UpdateSender(float dt);
	void Clear();

	virtual void OnReceive(Object& obj) {}
};

class Game : public GraphicsSender, public RENDERER::RendererUser, public APPLICATION::AudioUser
{
public:
	enum RETURNCODE { CONTINUE, RETRYLVL, NEXTLVL };
	MATH::Vec2 OffsetVec;

	static Controls DefaultControls;
private:
	BITMAP* Buffer;
	BITMAP* Mask;

	int Flow;

	float Time;
	bool HUD;
	
	Controls UserControls;

	FRAMEWORK::Keyboard* KeyboardObj;

	World WorldObj;
	
	Background Backimage; 
	
	BITMAP* Star;
	BITMAP* BonusSprite;
	BITMAP* BodySprite;
	BITMAP* BoxSprite;
	BITMAP* Finish;

	OutputStreamPtr StarSound;
	OutputStreamPtr ScreamSound;
	OutputStreamPtr BonusSound;
	OutputStreamPtr TurtleSound;
	OutputStreamPtr JumpSound;
	static OutputStreamPtr Music;
	

	RENDERER::AnimateSprite anim;
	RENDERER::AnimateSprite EnemyAnim;
	RENDERER::AnimateSprite BonusAnim;
	RENDERER::Sprite JumperSprite;
	RENDERER::Sprite Tileset[2];
	RENDERER::Sprite Particle;
	RENDERER::ParticleEmitter Particles;
	RENDERER::ParticleEmitterDesc ParticlesDesc;

	Rect PointsField;
	Rect LivesField;
	int MaxPoints;
	string MapFile;

	bool End;
	RETURNCODE ReturnCode;

	Timer<Game> FlowTimer;
	int flowDir;
public:
	static BITMAP* StarBig;
	static BITMAP* HeartBig;
	static BITMAP* Clock;
	

	Game() : End(false), flowDir(1), Flow(0) { UserControls = DefaultControls; }

	Object::TYPE GetNeighbour(Object* obj, int n);

	bool Collision(Object& obj1, Object& obj2, const Vec2& normal);
	Bonus::BONUSTYPE RandBonus();

	void StartBonus();
	void EndBonus();
	
	Controls& GetControls() { return UserControls; }
	static void SetMusic(OutputStreamPtr music) { Music = music; }
	void SetControls(const Controls& ctrls) { UserControls = ctrls;  }
	void RestartGame();
	void SetKeyboard(FRAMEWORK::Keyboard* kb) { KeyboardObj = kb; }
	void OnChangeGraphicsMode();
	void Init(const string& map, bool hud=true, const Vec2& OffsetVector = Vec2(0.0f, 0.0f));
	void SetMaxPoints(int mp) { MaxPoints = mp; }
	void FlowFunction();

	RETURNCODE Update(float dt);
	void Draw();
	void Release();

	void OnReceive(Object& obj);

	Player* GetPlayer() { return WorldObj.PlayerObj; }
	float GetTime() { return Time; }
};






};
using namespace GAME;