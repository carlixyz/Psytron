#pragma once
#include "GameState.h"
#include <raylib-cpp.hpp>
#include <map>
#include "./reasings.h"
//#include "../../Utility/Utils.h"


class DialogState : public GameState
{
	struct ImageMove : public raylib::TextureUnmanaged
	{
		bool Visible = true;	// Determines whether our Texture should be visible or hidden.
		float PositionX = 0;	// Current Horizontal position in Screen
		float PositionY = 0;	// Current Horizontal position in Screen
		float Alpha = 1.0f;		// Current Alpha Value
		::Vector2 Size {};

		float StartValue = 0.f;
		float EndValue = 1.0f;
		float CurrentTime = 0.0f;
		float TotalTime = 3.0f;
		bool Completed = false;
				
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
			//Alpha = EaseLinearOut((TotalTime, StartPosition, EndPosition - StartPosition, Duration);
			Alpha = EaseCubicOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionBounceIn()
		{
			PositionX = EaseElasticOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
		}

		void ActionSlideInScreen()
		{
			Alpha = EaseSineOut(CurrentTime, 0.0f, 1.0f, TotalTime);
			PositionX = EaseExpoOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
			Completed = CurrentTime > TotalTime ? true : false;
		}

		void ActionSwipeThroughScreen()
		{
			//Alpha = EaseSineOut((float)TotalTime, 0.0f, 1.0f, Duration);
			//Alpha = EaseCircOut((float)TotalTime, 0.0f, 1.0f, Duration);
			Alpha = EaseSineInOut(CurrentTime, 0.0f, 1.0f, TotalTime);
			PositionX = EaseCubicOut(CurrentTime, StartValue, EndValue - StartValue, TotalTime);
		}

		typedef void (ImageMove::*funPtr)(void);	/// Don't get scaried, this is just a function pointer
		funPtr Easing = nullptr;

		void ExecuteEasing()						
		{
			if (Completed)
				return;

			CurrentTime += GetFrameTime();

			if (Easing)
				(this->*Easing)();					/// We're using it here just to simplify things
		}

		inline void Draw()
		{
			if (Visible && IsReady())
				::DrawTextureV(*this, {PositionX, PositionY}, Fade(WHITE, Alpha));
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

		///  All these below can be removed and added right in DialogState::SetEasing() in .cpp
		inline void SetFadeIn()	
		{
			Easing = &ImageMove::ActionFade;
			StartValues(0.0f, 1.0f, 3);
		}
	};

public:

	std::map<std::string, ImageMove*> ImagesMap; // = { {"ActorLeft", new ImageMove()}, {"ActorRight", new ImageMove()} };

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();
		
	void LoadImage(std::string imageId, std::string file);			//	Create Image ID from a file

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

	enum EActionEasing
	{
		EActionNone = 0,
		EFadeIn,
		EFadeOut,
		ESlideLeft,
		ESlideRight,
		ESwipeLeftOut,
		ESwipeRightOut
	};

	void SetEasing(std::string imageId, EActionEasing easing = EActionEasing::EActionNone);

	void SetFullSize(std::string imageId, bool fullSize = true);
};

