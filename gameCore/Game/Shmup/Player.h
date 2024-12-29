#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Player : public Entity
{
	Player();

	void Update();
	void Render();

	float Speed					{};
	int Shield					{ 5 };

	Rectangle FrameOutput		{};

	float LeftMargin			= 86 * Graphics::Get().GetFactorArea().x;
	const float RightMargin		= 486 * Graphics::Get().GetFactorArea().x;
	const float BottomMargin	= 432 * Graphics::Get().GetFactorArea().y;
	const float OutMargin		= Graphics::Get().GetWindowArea().height + 256;

	bool IntroMode				{ true };
	bool ApplyDamage();

private:

	void UpdateDamage();


	void Shoot();
	bool bDebug{};

	int CurrentFrame			= 0;
	float CurrentDelay			= 0.0f;
	const float DelayTime		= 0.1f;// 0.025f; // 0.5f;
	float SoundDelay			= 0.35f;

	int goDirection				= 0;
	unsigned framesHoldingShot	= 0;
	float slowMove				= 1.0f;

	Vector2 ShotInitialOffset	= Vector2(40 * Graphics::Get().GetFactorArea().x, 24 * Graphics::Get().GetFactorArea().y);
	Vector2 ColliderOffset		{};
	Vector2 OldPosition			= Vector2Zero();

	/// Process Sprite flickering when entity get damaged
	bool DoDamageFlash = false;
	bool ShowSpriteFlash = false;
	float CurrentFlashTime = 0.0f;
	float totalFlashTime = 2.0f;
};

