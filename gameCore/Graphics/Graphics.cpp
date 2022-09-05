#include "Graphics.h"

//#include <assert.h>

bool Graphics::Init(ApplicationProperties* appProperties)
{
 // Initialize the library 
	bool Result = true;

	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);    
	window = new raylib::Window(	appProperties->Width,
									appProperties->Height, 
									appProperties->ApplicationName);

	SetTargetFPS(appProperties->FPS);

	if (appProperties->Fullscreen) SwitchFullScreen();

	windowArea = {	GetWindowPosition().x,
					GetWindowPosition().y,
					(float)appProperties->Width,
					(float)appProperties->Height };

	return Result;
}

bool Graphics::Deinit()
{
	if (window)
		CloseWindow();

	delete window;

	return true;
}

void Graphics::SwitchFullScreen()
{
	// Don't mess with specs just toggle the state
	window->ToggleFullscreen();

	if ( !window->IsFullscreen())	//if (IsWindowFullscreen())
	{
		// if we are full screen, then go back to the windowed size
		window->SetSize((int)windowArea.width, (int)windowArea.height);
		window->SetPosition((int)windowArea.x, (int)windowArea.y);
	}
	else
	{
		// see what display we are on right now
		int display = GetCurrentMonitor();
		// if we are not full screen, set the window size to match the monitor we are on
		window->SetSize(GetMonitorWidth(display), GetMonitorHeight(display));
	}
}

bool Graphics::GetCloseApplication()
{
	CloseApplication = window->ShouldClose();
	return CloseApplication;
}
