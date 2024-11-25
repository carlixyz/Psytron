#include "Graphics.h"

//#include <assert.h>

#define SCALED_WINDOW 1	// If true, Define a special factor to scale X2 all sprites 

bool Graphics::Init(ApplicationProperties* appProperties)
{
 // Initialize the library 23
	bool Result = true;

	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	Window = new raylib::Window(	appProperties->Width,
									appProperties->Height, 
									appProperties->ApplicationName);
	SetTargetFPS(appProperties->FPS);

	if (appProperties->Fullscreen) SwitchFullScreen();

	WindowArea = {	GetWindowPosition().x,
					GetWindowPosition().y,
					(float)appProperties->Width,
					(float)appProperties->Height };

#ifdef SCALED_WINDOW
	ScaledFactor = { GetWindowArea().width / NativeArea.x,			// 1280 / 640 = 2
						GetWindowArea().height / NativeArea.y };	// 960	/ 480 = 2;
#endif // SCALED_WINDOW

	return Result;
}

bool Graphics::Deinit()
{
	if (Window)
		CloseWindow();

	delete Window;

	return true;
}

void Graphics::SwitchFullScreen()
{
	// Don't mess with specs just toggle the state
	Window->ToggleFullscreen();

	if ( !Window->IsFullscreen())	//if (IsWindowFullscreen())
	{
		// if we are full screen, then go back to the windowed size
		Window->SetSize((int)WindowArea.width, (int)WindowArea.height);
		Window->SetPosition((int)WindowArea.x, (int)WindowArea.y);
	}
	else
	{
		// see what display we are on right now
		int display = GetCurrentMonitor();
		// if we are not full screen, set the window size to match the monitor we are on
		Window->SetSize(GetMonitorWidth(display), GetMonitorHeight(display));
	}
}

bool Graphics::GetCloseApplication()
{
	CloseApplication = Window->ShouldClose();
	return CloseApplication;
}
