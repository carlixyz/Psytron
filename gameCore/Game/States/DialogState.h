#pragma once
#include "GameState.h"
#include <raylib-cpp.hpp>
#include <unordered_map>
#include <map>
#include "./reasings.h"
#include "../../Graphics/Graphics.h"
//#include <functional>

/// TODO: General Code CleanUp here and Cpp

class DialogState : public GameState
{
	struct ImageEase : public raylib::TextureUnmanaged
	{
		ImageEase(std::string id) : Id(id) {}

		bool Visible		= true;							// Determines whether our Texture should be visible or hidden.
		float PositionX		= 0;							// Current Horizontal position in Screen
		float PositionY		= 0;							// Current Vertical position in Screen
		float Alpha			= 1.0f;							// Current Alpha Color Value
		Rectangle Size		= { 0.0f, 0.0f, 1.0f, 1.0f };
		Rectangle FileSize	= { 0.0f, 0.0f, 1.0f, 1.0f };	// Original Image Size before resizing

		float StartValue	= 0.f;
		float EndValue		= 1.0f;
		float CurrentTime	= 0.0f;
		float TotalTime		= 3.0f;
		bool Completed		= false;
		std::string Id		= "";
				
		GETTERSETTER(bool, IsVisible, Visible)
		GETTERSETTER(float, Alpha, Alpha)

		void StartValues(float startPosition, float endPosition, float duration = 3.0f)
		{
			StartValue = startPosition;
			EndValue = endPosition;
			TotalTime = duration;
			CurrentTime = 0.0f;
			Completed = false;
			Visible = true;
		}

		void ActionFade()
		{
			Alpha = EaseCubicOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime < TotalTime ? false : true;
		}

		void ActionScroll()
		{
			Size.x = EaseLinearIn(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
		}

		void ActionScrollClamp()
		{
			Alpha = EaseCubicOut(CurrentTime, 0.0f, 1.0f, 2.0);
			Size.x = EaseLinearOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = Size.x > 0 && Graphics::Get().GetWindowArea().width + Size.x < Size.width ? false : true;
		}

		void ActionScrollLoop()
		{
			Alpha = EaseCubicOut(CurrentTime, 0.0f, 1.0f, 2.0);
			Size.x = EaseSineIn(CurrentTime, StartValue, EndValue - StartValue, TotalTime) * 0.5f;
		}

		void ActionShake()
		{
			if (CurrentTime < TotalTime * 0.5f)
				Size.x = -(EndValue * 0.5f) + EaseSineInOut(CurrentTime, StartValue, EndValue - StartValue, .1f);
			else
				Size.x = EaseElasticInOut(CurrentTime, EndValue, StartValue - EndValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionSlide()
		{
			PositionX = EaseExpoOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionSlideInScreen()
		{
			Alpha = EaseSineOut(CurrentTime, 0.0f, 1.0f, TotalTime);
			PositionX = EaseExpoOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionSlideOutScreen()
		{
			Alpha = EaseSineOut(CurrentTime, Alpha, 0.0f -Alpha, 1.0f);
			PositionX = EaseExpoOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionSwipeThroughScreen()
		{
			//Alpha = EaseCircOut((float)TotalTime, 0.0f, 1.0f, Duration);
			Alpha = EaseSineInOut(CurrentTime, 0.0f, 1.0f, TotalTime);
			PositionX = EaseCubicOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
		}

		typedef void (ImageEase::*funPtr)(void);	/// Don't get scaried, this is just a function pointer
		funPtr Easing = nullptr;

		void ExecuteEasing()						
		{
			if (Completed)
			{
				if (Easing) Easing = nullptr;					
				return;
			}

			CurrentTime += GetFrameTime();

			if (Easing)
				(this->*Easing)();					/// We're using it here just to simplify things

		}

		inline void Draw()
		{
			if (Visible && IsReady())
				::DrawTextureRec(*this, Size, {PositionX, PositionY}, Fade(WHITE, Alpha));
				//::DrawTextureV(*this, {PositionX, PositionY}, Fade(WHITE, Alpha));
		}

		// Draw a Texture2D with position defined as Vector2
		inline void Draw(::Vector2 position, ::Color tint)
		{
			if (Visible && IsReady())
				::DrawTextureV(*this, position, tint);
		}

		// Draw a Texture2D with extended parameters
		inline void Draw(::Vector2 position, float rotation, float scale = 1.0f, ::Color tint = WHITE)
		{
			if (Visible && IsReady())
				::DrawTextureEx(*this, position, rotation, scale, tint);
		}
	};

public:

	std::unordered_map<std::string, ImageEase*> ImagesMap; // = { {"ActorLeft", new ImageMove()}, {"ActorRight", new ImageMove()} };
	std::vector<ImageEase*> ImagesVector;
	//std::map<std::string, ImageEase*> ImagesMap; // = { {"ActorLeft", new ImageMove()}, {"ActorRight", new ImageMove()} };

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();
		
	void LoadImage(std::string imageId, std::string file);			//	Create Image ID from a file

	void Unload(std::string imageId);								// Clean Up one Image

	void UnLoadAll();													// Clean Up ALL

	void SetImageVisible(std::string imageId, bool enabled = true);	// ShowImage ID, HideImage ID

	enum EScreenPosition
	{
		EPositionCenter = 0,
		EPositionLeft,
		EPositionRight,
		EPositionLeftOut,
		EPositionRightOut
	};
	
	void SetPosition(std::string imageId, EScreenPosition position = EScreenPosition::EPositionCenter );

	void MovePosition(std::string imageId, EScreenPosition position = EScreenPosition::EPositionCenter );

	enum EActionEasing
	{
		EActionNone = 0,
		EFadeIn,
		EFadeOut,
		ESlideFromLeft,
		ESlideFromRight,
		EScrollLeft,
		EScrollRight,
		EScrollLeftCap,
		EScrollRightCap,
		EScrollCycle,
		EShakeQuake
	};

	void SetEasing(std::string imageId, EActionEasing easing = EActionEasing::EActionNone);

	enum EImageSize
	{
		ENormal = 0,
		EStrechProportion,
		EExtend,
		//EStrechAll
	};

	void SetFullSize(std::string imageId, EImageSize formatSize = EStrechProportion);
};

