#define ALLEGRO_HAVE_STDINT_H
#include<allegro.h>
#include"Framework/Messages.h"
#include"Application/Application.h"


//Game game;
Application App;


int main() 
{
	#if defined _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(292);
	#endif // _DEBUG

	
	FRAMEWORK::MsgManager.AddMessageListener(App);
	App.Init(RENDERER::GraphicsSettings(800, 600, 32));
	App.Run();
	App.Release();

	return 0;
}
END_OF_MAIN();