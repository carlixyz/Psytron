#include "DialogState.h"
#include "../ConversationManager.h"
#include "../../Graphics/Graphics.h"

#define PADDING 100
#define POS_CENTER ((Graphics::Get().GetWindowArea().width * 0.5f) - (ImagesMap[imageId]->GetWidth() * 0.5f))
#define POS_LEFT (PADDING)
#define POS_RIGHT (Graphics::Get().GetWindowArea().width - ImagesMap[imageId]->GetWidth() - PADDING)
#define POS_LEFT_OUT ((float)-ImagesMap[imageId]->GetWidth() - PADDING)
#define POS_RIGHT_OUT (Graphics::Get().GetWindowArea().width + PADDING)

void DialogState::OnInit()
{

	ConversationManager::Get().Init("Data/dialogTest.yml");
	//ConversationManager::Get().StartConversation("Comienzo"); // Activar este para realizar una comica charla al iniciar el juego
	ConversationManager::Get().StartConversation("Setup");		// Activar este para realizar una interesante charla con acciones y logica
}

void DialogState::OnDeinit()
{
	for (auto& img : ImagesMap)
	{
		img.second->SetIsVisible(false);
		img.second->Unload();
		delete img.second;
	}

	ImagesMap.clear();
}

void DialogState::OnUpdate()
{
	for (auto const& img : ImagesMap)
	{
		img.second->ExecuteEasing();
	}
}

void DialogState::OnRender()
{
	//for (auto const& img : ImagesMap)
	//{
	//	img.second->Draw();
	//}

	// Iterate in reverse to draw stuff in order of creation
	for (auto it = ImagesMap.rbegin(); it != ImagesMap.rend(); it++) 
	{
		it->second->Draw();
	}
}

void DialogState::LoadImage(std::string imageId, std::string file)
{
	//if(ImagesMap.find(imageId) != ImagesMap.end())
	//if (ImagesMap[imageId] == nullptr)
	if(!ImagesMap.contains(imageId))
		ImagesMap[imageId] = new ImageMove();

	ImagesMap[imageId]->Load(file);
	//ImagesMap[imageId]->SetAlpha(0.0f);

	ImagesMap[imageId]->Size.x = (float)ImagesMap[imageId]->GetWidth();
	ImagesMap[imageId]->Size.y = (float)ImagesMap[imageId]->GetHeight();

	ImagesMap[imageId]->PositionX = (Graphics::Get().GetWindowArea().width * 0.5f) - (ImagesMap[imageId]->GetWidth() * 0.5f);
	ImagesMap[imageId]->PositionY = (Graphics::Get().GetWindowArea().height - ImagesMap[imageId]->GetHeight());
}

void DialogState::SetImageVisible(std::string imageId, bool enabled)
{
	if (ImagesMap.contains(imageId))
		ImagesMap[imageId]->SetIsVisible(enabled);
}

void DialogState::SetPosition(std::string imageId, EScreenPosition position)
{
	if (!ImagesMap.contains(imageId))
		return;	

	switch (position)
	{
		case EScreenPosition::EPositionLeft:
			ImagesMap[imageId]->PositionX = PADDING;
			break;

		case EScreenPosition::EPositionLeftOut:
			ImagesMap[imageId]->PositionX = (float)-ImagesMap[imageId]->GetWidth() - PADDING;
			break;

		case EScreenPosition::EPositionRight:
			ImagesMap[imageId]->PositionX = Graphics::Get().GetWindowArea().width - ImagesMap[imageId]->GetWidth() - PADDING;
			break;

		case EScreenPosition::EPositionRightOut:
			ImagesMap[imageId]->PositionX = Graphics::Get().GetWindowArea().width + PADDING;
			break;

		case EScreenPosition::EPositionCenter:
		default:
			ImagesMap[imageId]->PositionX = (Graphics::Get().GetWindowArea().width * 0.5f) - (ImagesMap[imageId]->GetWidth() * 0.5f);
			break;
	}
}

void DialogState::SetEasing(std::string imageId, EActionEasing easing)
{
	if (!ImagesMap.contains(imageId))
		return;

	switch (easing)
	{
		case EActionEasing::EFadeIn:
			ImagesMap[imageId]->Easing = &ImageMove::ActionFade;
			ImagesMap[imageId]->Alpha = 0.0f;
			ImagesMap[imageId]->StartValues(0.0f, 1.0f, 2);
			break;

		case EActionEasing::EFadeOut:
			ImagesMap[imageId]->Easing = &ImageMove::ActionFade;
			ImagesMap[imageId]->Alpha = 1.0f;
			ImagesMap[imageId]->StartValues(1.0f, 0.0f, 2);
			break;

		case EActionEasing::ESlideLeft:
			ImagesMap[imageId]->Easing = &ImageMove::ActionSlideInScreen;
			ImagesMap[imageId]->PositionX = POS_LEFT_OUT;
			ImagesMap[imageId]->StartValues(POS_LEFT_OUT, POS_CENTER, 2);
			break;

		case EActionEasing::ESlideRight:
			ImagesMap[imageId]->Easing = &ImageMove::ActionSlideInScreen;
			ImagesMap[imageId]->PositionX = POS_RIGHT_OUT;
			ImagesMap[imageId]->StartValues(POS_RIGHT_OUT, POS_CENTER, 2);
			break;

		case EActionEasing::ESwipeLeftOut:
			ImagesMap[imageId]->Easing = &ImageMove::ActionSwipeThroughScreen;
			ImagesMap[imageId]->PositionX = POS_LEFT_OUT;
			ImagesMap[imageId]->StartValues(POS_LEFT_OUT, POS_CENTER, 3);
			break;

		case EActionEasing::ESwipeRightOut:
			ImagesMap[imageId]->Easing = &ImageMove::ActionSwipeThroughScreen;
			ImagesMap[imageId]->PositionX = POS_RIGHT_OUT;
			ImagesMap[imageId]->StartValues(POS_RIGHT_OUT, POS_CENTER, 3);
			break;

		case EActionEasing::EActionNone:
		default:
			ImagesMap[imageId]->Completed = true;
			ImagesMap[imageId]->CurrentTime = 0.f;
			break;
	}

}

void DialogState::SetFullSize(std::string imageId, bool fullSize)
{
	if (!ImagesMap.contains(imageId))
		return;

	if (fullSize)
	{
		ImagesMap[imageId]->SetWidth((int)Graphics::Get().GetWindowArea().width);
		ImagesMap[imageId]->SetHeight((int)Graphics::Get().GetWindowArea().height);
		//ImagesMap[imageId]->PositionY = 0;
	}
	else
	{
		ImagesMap[imageId]->SetWidth((int)ImagesMap[imageId]->Size.x);
		ImagesMap[imageId]->SetHeight((int)ImagesMap[imageId]->Size.y);
	}

	ImagesMap[imageId]->PositionX = (Graphics::Get().GetWindowArea().width * 0.5f) - (ImagesMap[imageId]->GetWidth() * 0.5f);
	ImagesMap[imageId]->PositionY = Graphics::Get().GetWindowArea().height - ImagesMap[imageId]->GetHeight();

}

