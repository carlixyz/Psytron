
#if defined _WIN32 && !_DEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include "Game/Game.h"

int main()
{
    Game::Get().Init();

    while (!Game::Get().HasFinished())
    {
        Game::Get().Update();

        Game::Get().Render();
    }

    Game::Get().Deinit();

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}