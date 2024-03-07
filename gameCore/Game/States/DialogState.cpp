#include "DialogState.h"
#include "../ConversationManager.h"


void DialogState::OnInit()
{
	ConversationManager::Get().Init(Game::Get().appProperties.DialogFilePath); // .Init("Data/dialogTest.yml");
	//ConversationManager::Get().StartConversation("EnterPsyTron");
	ConversationManager::Get().StartConversation("IntroSchool");
	//ConversationManager::Get().StartConversation("CodeTests");		// Activar este para realizar una interesante charla con acciones y logica
}

void DialogState::OnDeinit()
{
}


void DialogState::OnUpdate()
{
	//if (ConversationManager::Get().HasEndedConversation())
	if (!ConversationManager::Get().IsInConversation())
	{
		//Game::Get().States.ChangeState(Game::Get().States.titleState);
		Game::Get().States.PopState();
	}
}

void DialogState::OnRender()
{
}
