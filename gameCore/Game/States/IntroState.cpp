#include "IntroState.h"
#include "../Game.h"
#include "../ConversationManager.h"
#include "../VisualDialogManager.h"

void IntroState::OnInit()
{
	//* Add any of your logos
	ConversationManager::Get().Init(Game::Get().appProperties.DialogFilePath);
	ConversationManager::Get().StartConversation("InfoNews");
}

void IntroState::OnDeinit()
{
	VisualDialogManager::Get().UnLoadAll();
}

void IntroState::OnUpdate()
{
	if (ConversationManager::Get().HasEndedConversation())
	{
		Game::Get().States.ChangeState(Game::Get().States.titleState);
	}
}

void IntroState::OnRender()
{

}


//void IntroState::OnPause()
//{
//}

//void IntroState::OnResume()
//{
//}
