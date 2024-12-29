#include "VisualDialogManager.h"
#include "ConversationManager.h"
#include "../Graphics/Graphics.h"
#include <functional>

#define PADDING 50
#define POS_CENTER ((Graphics::Get().GetHorizontalCenter()) - (ImagesMap[imageId]->Size.width * 0.5f))
#define POS_LEFT (PADDING)
#define POS_RIGHT (Graphics::Get().GetWindowArea().width - ImagesMap[imageId]->Size.width - PADDING)
#define POS_LEFT_OUT ((float)-ImagesMap[imageId]->Size.width - PADDING)
#define POS_RIGHT_OUT (Graphics::Get().GetWindowArea().width + PADDING)

void VisualDialogManager::Init()
{
	//ConversationManager::Get().Init(Game::Get().appProperties.DialogFilePath); // .Init("Data/dialogTest.yml");
	//ConversationManager::Get().StartConversation("Comienzo"); // Activar este para realizar una comica charla al iniciar el juego
	//ConversationManager::Get().StartConversation("CodeTests");		// Activar este para realizar una interesante charla con acciones y logica
}

void VisualDialogManager::Deinit()
{
	UnLoadAll();
}

void VisualDialogManager::UnLoadAll()
{	
	for (auto& img : ImagesMap)
	{
		img.second->SetIsVisible(false);
		img.second->Unload();
		delete img.second;
	}

	ImagesMap.clear();
	ImagesVector.clear();
}

void VisualDialogManager::Unload(std::string imageId)
{
	if (!ImagesMap.contains(imageId))
		return;

	for (std::vector<ImageEase*>::iterator it = ImagesVector.begin(); it != ImagesVector.end(); it++)
	{
		if ((*it)->Id == imageId)
		{
			ImagesMap[imageId]->SetIsVisible(false);
			ImagesMap[imageId]->Unload();
			ImagesMap.erase(imageId);		// Remove Dict remaining item

			delete (*it);					// Delete Obj

			it = ImagesVector.erase(it);	// Remove vector remaining item

			return;
		}
	}
}

void VisualDialogManager::Update()
{
	for (auto const& img : ImagesVector)
		img->ExecuteEasing();
}

void VisualDialogManager::Render()
{
	for (auto const& img : ImagesVector)
		img->Draw();
}

void VisualDialogManager::LoadImage(std::string imageId, std::string file)
{
	if (!ImagesMap.contains(imageId))
	{
		ImagesMap[imageId] = new ImageEase(imageId);
		ImagesVector.push_back(ImagesMap[imageId]);
	}

	ImagesMap[imageId]->Load(file);
	//---------------------------------------------------

	SetFullSize(imageId, EImageSize::EScaleResolution);	

	//---------------------------------------------------
	ImagesMap[imageId]->FileSize.width = ImagesMap[imageId]->Size.width;
	ImagesMap[imageId]->FileSize.height = ImagesMap[imageId]->Size.height;

	ImagesMap[imageId]->PositionX = (Graphics::Get().GetHorizontalCenter()) - (ImagesMap[imageId]->Size.width * 0.5f);
	ImagesMap[imageId]->PositionY = (Graphics::Get().GetWindowArea().height - ImagesMap[imageId]->Size.height);
}

void VisualDialogManager::SetImageVisible(std::string imageId, bool enabled)
{
	if (ImagesMap.contains(imageId))
	{
		ImagesMap[imageId]->SetIsVisible(enabled);
	}
}

void VisualDialogManager::SetPosition(std::string imageId, EScreenPosition position)
{
	if (!ImagesMap.contains(imageId))
		return;	

	switch (position)	// This jumps our image right into a new position
	{
		case EScreenPosition::EPositionLeft:
			if (ImagesMap[imageId]->Size.width > Graphics::Get().GetWindowArea().width) // If image is wider than screen then it's a BG
				ImagesMap[imageId]->PositionX = 0;
			else
				ImagesMap[imageId]->PositionX = PADDING;
			break;

		case EScreenPosition::EPositionLeftOut:
			ImagesMap[imageId]->PositionX = (float)-ImagesMap[imageId]->Size.width - PADDING;
			break;

		case EScreenPosition::EPositionRight:
  			if (ImagesMap[imageId]->Size.width > Graphics::Get().GetWindowArea().width) // If image is wider than screen then it's a BG
				ImagesMap[imageId]->PositionX = Graphics::Get().GetWindowArea().width - ImagesMap[imageId]->Size.width;
			else 
				ImagesMap[imageId]->PositionX = Graphics::Get().GetWindowArea().width - ImagesMap[imageId]->Size.width - PADDING;

			break;

		case EScreenPosition::EPositionRightOut:
			ImagesMap[imageId]->PositionX = Graphics::Get().GetWindowArea().width + PADDING;
			break;

		case EScreenPosition::EPositionCenter:
		default:
			ImagesMap[imageId]->PositionX = (Graphics::Get().GetHorizontalCenter()) - (ImagesMap[imageId]->Size.width * 0.5f);
			break;
	}
}

void VisualDialogManager::MovePosition(std::string imageId, EScreenPosition position)
{
	if (!ImagesMap.contains(imageId))	// Moves our image towards a new position with a smooth easing 
		return;

	bool isOutScreen = ImagesMap[imageId]->PositionX < -PADDING ||
		ImagesMap[imageId]->PositionX > Graphics::Get().GetWindowArea().width + PADDING;
	
	switch (position)	// This Slide something from current position to a new one.
	{
		case EScreenPosition::EPositionLeft:
			ImagesMap[imageId]->Easing = isOutScreen ? &ImageEase::ActionSlideInScreen : &ImageEase::ActionSlide;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->PositionX, POS_LEFT, 2);
			break;

		case EScreenPosition::EPositionLeftOut:
			ImagesMap[imageId]->Easing = &ImageEase::ActionSlideOutScreen;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->PositionX, POS_LEFT_OUT, 2);
			break;

		case EScreenPosition::EPositionRight:
			ImagesMap[imageId]->Easing = isOutScreen ? &ImageEase::ActionSlideInScreen : &ImageEase::ActionSlide;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->PositionX, POS_RIGHT, 2);
			break;

		case EScreenPosition::EPositionRightOut:
			ImagesMap[imageId]->Easing = &ImageEase::ActionSlideOutScreen;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->PositionX, POS_RIGHT_OUT, 2);
			break;

		case EScreenPosition::EPositionCenter:
		default:
			ImagesMap[imageId]->Easing = isOutScreen ? &ImageEase::ActionSlideInScreen : &ImageEase::ActionSlide;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->PositionX, POS_CENTER, 2);
			break;
	}
}

void VisualDialogManager::SetEasing(std::string imageId, EActionEasing easing)
{
	if (!ImagesMap.contains(imageId))
		return;

	switch (easing)
	{
		case EActionEasing::EFadeIn:
			ImagesMap[imageId]->Alpha = 0.0f;
			ImagesMap[imageId]->Easing = &ImageEase::ActionFade;
			ImagesMap[imageId]->StartValues(0.0f, 1.0f, 3);
			break;

		case EActionEasing::EFadeOut:
			ImagesMap[imageId]->Alpha = 1.0f;
			ImagesMap[imageId]->Easing = &ImageEase::ActionFade;
			ImagesMap[imageId]->StartValues(1.0f, 0.0f, 3);
			break;

		case EActionEasing::ESlideFromLeft:
			ImagesMap[imageId]->PositionX = POS_LEFT_OUT;
			MovePosition(imageId);
			break;

		case EActionEasing::ESlideFromRight:
			ImagesMap[imageId]->PositionX = POS_RIGHT_OUT;
			MovePosition(imageId);
			break;

		case EActionEasing::EScrollLeft:
			ImagesMap[imageId]->Easing = &ImageEase::ActionScroll;
			ImagesMap[imageId]->StartValues(100, 0, 1.0f);
			break;

		case EActionEasing::EScrollRight:
			ImagesMap[imageId]->Easing = &ImageEase::ActionScroll;
			ImagesMap[imageId]->StartValues(0, 100, 1.0f);
			break;

		case EActionEasing::EScrollLeftCap:
			ImagesMap[imageId]->PositionX = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollClamp;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->Size.width - Graphics::Get().GetWindowArea().width,	0, 30.0f);
			break;

		case EActionEasing::EScrollRightCap:
			ImagesMap[imageId]->PositionX = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollClamp;
			ImagesMap[imageId]->StartValues(0, ImagesMap[imageId]->Size.width - Graphics::Get().GetWindowArea().width, 30.0f);
			break;

		case EActionEasing::EScrollCycle:
			ImagesMap[imageId]->PositionX = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollLoop;
			ImagesMap[imageId]->StartValues(0, ImagesMap[imageId]->Size.width - Graphics::Get().GetWindowArea().width, 10.0f);
			break;

		case EActionEasing::EScrollTopCap:
			ImagesMap[imageId]->PositionY = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollClampVertical;
			ImagesMap[imageId]->StartValues(ImagesMap[imageId]->Size.height - Graphics::Get().GetWindowArea().height, 0, 30.0f);
			break;

		case EActionEasing::EScrollBottomCap:
			ImagesMap[imageId]->PositionY = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollClampVertical;
			ImagesMap[imageId]->StartValues(0, ImagesMap[imageId]->Size.height - Graphics::Get().GetWindowArea().height, 30.0f);
			break;

		case EActionEasing::EScrollCycleVertical:
			ImagesMap[imageId]->PositionY = 0;
			ImagesMap[imageId]->Easing = &ImageEase::ActionScrollLoopVertical;
			ImagesMap[imageId]->StartValues(0, ImagesMap[imageId]->Size.height - Graphics::Get().GetWindowArea().height, 10.0f);
			break;

		case EActionEasing::EShakeQuake:
			SetFullSize(imageId);
			ImagesMap[imageId]->Easing = &ImageEase::ActionShake;
			ImagesMap[imageId]->StartValues(0, 50, 2.0f);
			break;

		case EActionEasing::EActionNone:
		default:
			ImagesMap[imageId]->Completed = true;
			ImagesMap[imageId]->CurrentTime = 0.f;
			break;
	}

}

void VisualDialogManager::SetFullSize(std::string imageId, EImageSize formatSize)
{
	if (!ImagesMap.contains(imageId))
		return;

	if (formatSize == EImageSize::EStrechProportion) // Scale up to Viewport height and then rescale width in proportion
	{
		if (ImagesMap[imageId]->Size.width > ImagesMap[imageId]->Size.height) 
		{
			float proportionalWidth = ImagesMap[imageId]->Size.width * 
				Graphics::Get().GetWindowArea().height / ImagesMap[imageId]->Size.height;

			ImagesMap[imageId]->SetWidth((int)proportionalWidth);
			ImagesMap[imageId]->SetHeight((int)Graphics::Get().GetWindowArea().height);

			ImagesMap[imageId]->Size.width = proportionalWidth;
			ImagesMap[imageId]->Size.height = Graphics::Get().GetWindowArea().height;
		}
		else // if the image is taller than it width then invert proportions
		{
			float proportionalHeight = ImagesMap[imageId]->Size.height *
				Graphics::Get().GetWindowArea().width / ImagesMap[imageId]->Size.width;

			ImagesMap[imageId]->SetWidth((int)Graphics::Get().GetWindowArea().width);
			ImagesMap[imageId]->SetHeight((int)proportionalHeight);

			ImagesMap[imageId]->Size.width = Graphics::Get().GetWindowArea().width;
			ImagesMap[imageId]->Size.height = proportionalHeight;
		}

	}
	else if (formatSize == EImageSize::EScaleResolution)	// Scale using a Factor calculated as: NewResolution / NativeResolution
	{
		ImagesMap[imageId]->Size.width = (float)ImagesMap[imageId]->GetWidth() * Graphics::Get().GetFactorArea().x;
		ImagesMap[imageId]->Size.height = (float)ImagesMap[imageId]->GetHeight() * Graphics::Get().GetFactorArea().y;

		ImagesMap[imageId]->SetWidth((int)ImagesMap[imageId]->Size.width);
		ImagesMap[imageId]->SetHeight((int)ImagesMap[imageId]->Size.height);
	}
	//else if (formatSize == EImageSize::EStrechAll) // stretch up widht & heigh to Viewport (looks ugly)
	//{
	//	ImagesMap[imageId]->SetWidth((int)Graphics::Get().GetWindowArea().width);
	//	ImagesMap[imageId]->SetHeight((int)Graphics::Get().GetWindowArea().height);

	//	ImagesMap[imageId]->Size.width = Graphics::Get().GetWindowArea().width;
	//	ImagesMap[imageId]->Size.height = Graphics::Get().GetWindowArea().height;
	//}
	else if (formatSize == EImageSize::EExtend)		// stretch up just UV map width & heigh to Viewport
	{
		ImagesMap[imageId]->Size.width = Graphics::Get().GetWindowArea().width;
		ImagesMap[imageId]->Size.height = Graphics::Get().GetWindowArea().height;

		ImagesMap[imageId]->SetWidth((int)ImagesMap[imageId]->FileSize.width);
		ImagesMap[imageId]->SetHeight((int)ImagesMap[imageId]->FileSize.height);
	}
	else // (formatSize == EImageSize::ENormal)		// Restore Texture original file size
	{
		ImagesMap[imageId]->SetWidth((int)ImagesMap[imageId]->FileSize.width);
		ImagesMap[imageId]->SetHeight((int)ImagesMap[imageId]->FileSize.height);

		ImagesMap[imageId]->Size.width = (float)ImagesMap[imageId]->GetWidth();
		ImagesMap[imageId]->Size.height = (float)ImagesMap[imageId]->GetHeight();
	}

	ImagesMap[imageId]->PositionX = (Graphics::Get().GetHorizontalCenter()) - (ImagesMap[imageId]->Size.width * 0.5f);
	ImagesMap[imageId]->PositionY = Graphics::Get().GetWindowArea().height - ImagesMap[imageId]->Size.height;
}

