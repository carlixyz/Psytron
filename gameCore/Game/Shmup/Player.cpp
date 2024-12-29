#include "Player.h"
#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Particles.h"
#include <../../../Utility/Utils.h>
#include "../Assets.h"
#include <iomanip>
#include <sstream>
#include <string>
#include "../../Audio/Audio.h"
#include "reasings.h"


#define MODULE_FULL 32
#define MODULE_HALF (MODULE_FULL/2)
#define MODULE_FOURTH (MODULE_HALF/2)


Player::Player()
{
	Speed = 300.f * Graphics::Get().GetFactorArea().y;
	Position = Vector2(Graphics::Get().GetScreenCenter().x , OutMargin);
	Shield = 5;

	//Position = Graphics::Get().GetScreenCenter();

	SpriteSize.x = (float)(GetAsset("Sprites").width / MODULE_HALF);
	SpriteSize.y = (float)(GetAsset("Sprites").height / MODULE_FOURTH);

	FrameRec = { 0 * SpriteSize.x, 0 * SpriteSize.y, SpriteSize.x, SpriteSize.y };

	FrameOutput = { Position.x, Position.y,
		FrameRec.width * Graphics::Get().GetFactorArea().x,
		FrameRec.height * Graphics::Get().GetFactorArea().y
	};

	ColliderOffset = { FrameOutput.width * 0.3f, FrameOutput.height * 0.35f };

	CollisionRec = {
		Position.x + ColliderOffset.x,
		Position.y + ColliderOffset.y,
		FrameOutput.width * 0.3f, FrameOutput.height * 0.3f
	};

	//Image imBase = LoadImageFromTexture(GetAsset("Sprites"));
	//Image imCopy = ImageCopy(imBase);
	//ImageColorBrightness(&imCopy, +100);
	//SpriteFlash.Load(imCopy);
	//LoadTextureFromImage(imCopy);
	CurrentFlashTime = totalFlashTime;
}

void Player::Update()
{
	CurrentDelay -= GetFrameTime();
	SoundDelay -= GetFrameTime();
	OldPosition = Position;
	goDirection = 0;
	slowMove = 1.0f;

	if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_LEFT_CONTROL))
	{
		Shoot();
		slowMove = ++framesHoldingShot > 5 ? 0.4f : 1.0f;
	}

	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		Position.x -= Speed * GetFrameTime() * slowMove;
		goDirection = -1;
	}

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		Position.x += Speed * GetFrameTime() * slowMove;
		goDirection = 1;
	}

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
	{
		Position.y -= Speed * GetFrameTime() * slowMove;
	}

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
	{
		Position.y += Speed * GetFrameTime() * slowMove;
	}

	if (IsKeyReleased(KEY_SPACE) && IsKeyReleased(KEY_LEFT_CONTROL))
	{
		framesHoldingShot = 0;
	}


	CurrentFrame = (int)(GetTime() * MODULE_HALF);
	CurrentFrame %= 4;

	const unsigned frameSide = (goDirection +1) * 4;

	// Left:	4 frames * -1 +1 
	// Center:	4 frames * 0  +1
	// Right:	4 frames * +1 +1

	FrameRec.x = (frameSide + CurrentFrame) * SpriteSize.x;

	FrameOutput.x = Position.x = Clamp(Position.x, LeftMargin, RightMargin);
	FrameOutput.y = Position.y = Clamp(Position.y, 0, IntroMode ? OutMargin : BottomMargin);

	CollisionRec.x = Position.x + ColliderOffset.x;
	CollisionRec.y = Position.y + ColliderOffset.y;

	if (IsKeyDown(KEY_ONE))
	{
		Particles::Get().Create(Vector2Add(Position, ShotInitialOffset), BehaviourType::EExplotion);
	}

	UpdateDamage();
}


bool Player::ApplyDamage()
{
	if (DoDamageFlash)
		return false;

	DoDamageFlash = true;
	Shield -= 1;

	if (Shield <= 0)
	{
		const Vector2 explotion(Position.x + FrameOutput.width * 0.5f, Position.y + FrameOutput.height * 0.5f);
		Particles::Get().Create(explotion, BehaviourType::EExplotion);
		Audio::Get().PlaySound("Data/Sound/Explode.wav");
		Active = false;
	}
	return true;
}

void Player::UpdateDamage()
{
	if (!DoDamageFlash)
		return;

	CurrentFlashTime -= GetFrameTime();
	ShowSpriteFlash = EaseSineInOut(CurrentFlashTime, 0.f, 1.0f, 0.05f) > 0.5f;

	if (CurrentFlashTime < 0)
	{
		DoDamageFlash = false;
		ShowSpriteFlash = false;
		CurrentFlashTime = totalFlashTime;
	}
}

void Player::Render()
{
	/*
	std::stringstream stream;
	stream << "Position " << std::fixed << std::setprecision(2) << Position.x << " " << Position.y;
	std::string Text = stream.str();
	DrawText(Text.c_str(), 400, 400, 16, WHITE);
	*/

	if (!ShowSpriteFlash)
		DrawTexturePro(GetAsset("Sprites"), FrameRec, FrameOutput, Vector2Zero(), 0.f, WHITE);  // Draw part of the texture

	if (Game::Get().IsDebugMode())
		DrawRectanglePro(CollisionRec, Vector2Zero(), 0.f, ALPHARED);
}

void Player::Shoot()
{
	//Audio::Get().PlaySound("Fire");
	//PlaySound(Assets::Get().GetSound("Fire"));     

	// https://www.youtube.com/watch?v=QQ3Yub9So2k&t=202s

	if (CurrentDelay > 0.0f)	
		return;

	CurrentDelay = DelayTime;

	Particles::Get().CreatePlayerShot(Vector2Add(Position, ShotInitialOffset));

	if (SoundDelay < 0)
	{
		SoundDelay = 0.01f; // = 0.1f; 0.325f;
		Audio::Get().PlaySound("Data/Sound/MiniGun_A.wav");
	}

}

#undef MODULE_FOURTH
#undef MODULE_HALF
#undef MODULE_FULL
