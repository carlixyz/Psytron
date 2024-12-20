#include "Player.h"
#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Particles.h"
#include <../../../Utility/Utils.h>
#include "../Assets.h"
#include <iomanip>
#include <sstream>
#include <string>

#define MODULE_FULL 32
#define MODULE_HALF (MODULE_FULL/2)
#define MODULE_FOURTH (MODULE_HALF/2)


Player::Player()
{
	Speed = 300.f * Graphics::Get().GetFactorArea().y;
	Position = Vector2((float)Graphics::Get().GetHorizontalCenter(), (float)Graphics::Get().GetVerticalCenter());

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

	Image imBase = LoadImageFromTexture(GetAsset("Sprites"));
	Image imCopy = ImageCopy(imBase);
	ImageColorBrightness(&imCopy, +1000);

	SpriteBright.Load(imCopy);	LoadTextureFromImage(imCopy);
}

void Player::Update()
{
	CurrentDelay -= GetFrameTime();
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
	FrameOutput.y = Position.y = Clamp(Position.y, 0, BottomMargin);

	CollisionRec.x = Position.x + ColliderOffset.x;
	CollisionRec.y = Position.y + ColliderOffset.y;

	//static bool justOnce = true;
	if (IsKeyDown(KEY_ONE))
	//if (IsKeyDown(KEY_ONE) && justOnce)
	{
		//justOnce = false;
		//Particles::Get().Create(Vector2Add(Position, Vector2(21, 0)), BehaviourType::ELockShot);
		//Particles::Get().Create(Vector2Add(Position, ShotInitialOffset), BehaviourType::ELockShot);
		Particles::Get().Create(Vector2Add(Position, ShotInitialOffset), BehaviourType::EExplotion);
	}


	if (IsKeyDown(KEY_FOUR))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ESpiralShot); ///
	}

	if (IsKeyDown(KEY_FIVE))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EMultiSpiralShot);
	}

	if (IsKeyDown(KEY_SIX))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ERingShot);
	}

	if (IsKeyDown(KEY_SEVEN))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EHanaShot);
	}

	if (IsKeyDown(KEY_EIGHT))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EStarShot);
	}

	if (IsKeyDown(KEY_NINE))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EArchShot);
	}
	//*/
}



void Player::Render()
{
	/*
	std::stringstream stream;
	stream << "Position " << std::fixed << std::setprecision(2) << Position.x << " " << Position.y;
	std::string Text = stream.str();
	DrawText(Text.c_str(), 400, 400, 16, WHITE);
	*/

	//if(CheckCollisionPointRec(Position, Graphics::Get().GetWindowArea()))
	//	DrawRectangle((int)Position.x, (int)Position.y, 42, 64, WHITE);
	//else
	//	DrawRectangle((int)Position.x, (int)Position.y, 42, 64, BLUE);

	//DrawTextureRec(GetAsset("Sprites"), FrameRec, Position, WHITE);  // Draw part of the texture
	DrawTexturePro(GetAsset("Sprites"), FrameRec, FrameOutput, Vector2Zero(), 0.f, WHITE);  // Draw part of the texture

	//DrawRectanglePro({ Position.x, Position.y, 32, 64}, Vector2Zero(), 0.f, ALPHARED);
	DrawRectanglePro(CollisionRec, Vector2Zero(), 0.f, ALPHARED);

	//DrawCircle(AimOffset.x, AimOffset.y, 5, RED);
}

void Player::Shoot()
{
	// https://www.youtube.com/watch?v=QQ3Yub9So2k&t=202s

	if (CurrentDelay > 0.0f)	return;

	CurrentDelay = DelayTime;


	Particles::Get().CreatePlayerShot(Vector2Add(Position, ShotInitialOffset));



	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ELinearShot);		/// -----
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ELockShot);		/// -----
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ESeekShot);		/// -----
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EMultiShot);		/// ----- 
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ESpiralShot);		/// ----- 
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EMultiSpiralShot);	/// -----
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EHanaShot);		/// -----
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ERingShot);		// ----- <
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EStarShot);		// ----- <
	//Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EArchShot);		/// -----
}
