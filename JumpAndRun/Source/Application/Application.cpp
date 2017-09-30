#include<allegro.h>
#include<fstream>
#include<algorithm>
#include"Application.h"

#include"ApplicationStates/MainMenuState.h"
#include"ApplicationStates/NewGameState.h"
#include"ApplicationStates/LevelChoiceState.h"
#include"ApplicationStates/LoadGameState.h"
#include"ApplicationStates/ControlsState.h"
#include"ApplicationStates/OptionsState.h"
#include"ApplicationStates/HighScoresState.h"

using namespace FRAMEWORK;
using namespace GAME;
using namespace RENDERER;

Application* ApplicationState::App = NULL;
AudioManager AudioUser::Audio;



void Application::AddAppState(ApplicationState* state)
{
	States.push_back(state);
	StatesCallCount.push_back(0);
}

void Application::CreateUser(const string& name)
{
	CurrentUser.Name = name;
	CurrentUser.Stats.resize(3);

	for (int i=0;i<3;i++)
	{
		CurrentUser.Stats[i].Enemies = 0;
		CurrentUser.Stats[i].Points = 0;
		CurrentUser.Stats[i].Time = 0;
	}
	
	CurrentUser.Save(false);
	
	fstream file;
	file.open("gfx\\users.txt", ios::out);
	for (int i=0;i<Users.size();i++)
	{
		file << Users[i];
		file << endl;
	}

	file << name;
	file.close();
}

void Application::PrepareLevelsData()
{
	fstream file;
	file.open("gfx\\maps.txt", ios::in);

	int size;
	file >> size;
	Levels.resize(size);

	for (int i=0;i<size;i++)
	{
		file >> Levels[i].File;
		file >> Levels[i].Points;
		file >> Levels[i].Enemies;
	}
	
	file.close();
}

void Application::LoadUsers()
{
	fstream file;
	file.open("gfx\\users.txt", ios::in);

	Users.clear();
	while(!file.eof())
	{
		string u;
		file >> u;
		Users.push_back(u);
	}
	
	file.close();
}

void Application::Reboot(GraphicsSettings& set)
{
	RendererObj.Release();
	RendererObj.Init(set);
	//RendererObj.ChangeGraphicsMode(set);
}

void Application::Init(GraphicsSettings& set)
{
	Controls::KeyLabels[0] = "w lewo";
	Controls::KeyLabels[1] = "w prawo";
	Controls::KeyLabels[2] = "skok";
	Controls::KeyLabels[3] = "naskok";
	
	LoadUsers();
	PrepareLevelsData();

	RendererObj.Init(set);
	Audio.Init();

	Audio.LoadStream("sfx\\music.mp3", Music);
	Audio.SetSettings(Audio.GetSettings());
	Music->play();
	Music->setRepeat(true);
	/*
	int s = -1;
	if (s = install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "") < 0)
	{
		allegro_message("Create audio device failed");
		set_volume(100, 100);
	}
	else
	{
		Music = load_sample("sfx\\music.mp3");

		if (!Music)
		{
			
			allegro_message("Load audio file failed");	
		}

		//play_sample(Music, 3, 0, 0, 0);
	}*/

	//RendererObj.Init(GraphicsSettings(1366, 768, 32, true));

	ApplicationState::App = this;
	RendererUser::RendererObj = &RendererObj;

	for (int i=0;i<ApplicationState::NUMSTATES;i++)
	{
		ApplicationState* AppState;
		switch((ApplicationState::APPSTATE)i)
		{
		case ApplicationState::MAINMENU: 
			AppState = new MainMenuState;
			break;
		case ApplicationState::NEWGAME: 
			AppState = new NewGameState;
			break;
		case ApplicationState::LEVELCHOICE: 
			AppState = new LevelChoiceState;
			break;
		case ApplicationState::HIGHSCORES: 
			AppState = new HighScoresState;
			break;
		case ApplicationState::GAME: 
			AppState = new GameState;
			break;
		case ApplicationState::LOADGAME: 
			AppState = new LoadGameState;
			break;
		case ApplicationState::OPTIONS: 
			AppState = new OptionsState;
			((OptionsState*)AppState)->Create(&RendererObj.GetSettings());
			break;
		case ApplicationState::CONTROLS: 
			AppState = new ControlsState;
			break;
		};

		AppState->Sleep();
		AddAppState(AppState);
		AddKeyboardListener(AppState);
	}

	((LevelChoiceState*)States[ApplicationState::LEVELCHOICE])->GameState = (GameState*)States[ApplicationState::GAME];
	((ControlsState*)States[ApplicationState::CONTROLS])->Create(((GameState*)States[ApplicationState::GAME])->GetGame());

	//OnRender();
	SetState(ApplicationState::MAINMENU);
}

void Application::SetState(ApplicationState::APPSTATE state)
{
	if (CurrentState) CurrentState->Sleep();

	CurrentState = States[state];
	CurrentState->OnOpen();
	CurrentState->Listen();
	if (StatesCallCount[state] == 0)
	{
		CurrentState->Init();
	}
	StatesCallCount[state]++;
}

void Application::OnQuit()
{
	ApplicationState::APPSTATE state = CurrentState->OnQuit();
	if (state != ApplicationState::QUIT && state != CurrentState->AppState)
		SetState(state);
	//else End = true;
}

void Application::OnUpdate(float dt)
{
	ApplicationState::APPSTATE appstate = CurrentState->Update(dt);

	if (appstate != CurrentState->AppState)
	{
		switch(appstate)
		{
		case ApplicationState::QUIT:
			End = true;
			break;
		default:
			SetState(appstate);
		}
	}
}


void Application::OnRender()
{
	RendererObj.Begin();
	CurrentState->Draw();
	RendererObj.End();
}

void Application::Release()
{
	Audio.Release();
	SurfacesManager::Release();

	RendererObj.Release();
	
	for (int i=0;i<States.size();i++)
	{
		if (StatesCallCount[i]) States[i]->Release();
		delete States[i];
	}	
}
