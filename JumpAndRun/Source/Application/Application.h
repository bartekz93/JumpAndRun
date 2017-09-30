#pragma once
#include"../Framework/Messages.h"
#include"../Framework/Math.h"
#include"../Framework/Framework.h"
#include"../Framework/Audio.h"
#include"../Renderer/SurfacesManager.h"
#include"../Renderer/Renderer.h"
#include"../Game/Game.h"
#include"../Game/Object.h"
#include"../Game/World.h"
#include"User.h"
#include"ApplicationState.h"


namespace APPLICATION
{
	class Application : public FRAMEWORK::Framework, public AudioUser
	{
	private:
		User CurrentUser;
		std::vector<LevelDesc> Levels;
	
		RENDERER::Renderer RendererObj;
	
		std::vector<ApplicationState*> States;
		std::vector<int> StatesCallCount;
	
		ApplicationState* CurrentState;
	
		void PrepareLevelsData();
	public:
		std::vector<string> Users;
		OutputStreamPtr Music;
	
		Application() {}
	
		void SetUser(User& user) { CurrentUser = user; }
		void SetState(ApplicationState::APPSTATE state);
	
		User*			GetCurrentUser()		{ return &CurrentUser; }
		LevelDesc		GetLevelDesc(int i)		{ return Levels[i]; }
		AudioManager*	GetAudio()				{ return &Audio; }
	
		void Reboot(RENDERER::GraphicsSettings& set);
		void CreateUser(const std::string& name);
		void LoadUsers();
	
		void AddAppState(ApplicationState* state);
		void Init(RENDERER::GraphicsSettings& set);
		void Release();
	
		void OnQuit();
		void OnUpdate(float dt);
		void OnRender();
	};
};
using namespace APPLICATION;