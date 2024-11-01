#include "Player.h"
#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Particles.h"
#include <../../../Utility/Utils.h>
#include "../Assets.h"
#include <iomanip>
#include <sstream>
#include <string>


Player::Player()
{

	Speed = 300.f;
	Position = Vector2( (float)Graphics::Get().GetHorizontalCenter(), (float)Graphics::Get().GetVerticalCenter());

	SpriteSize.x = (float)(GetAsset("Sprites").width / 16);
	SpriteSize.y = (float)(GetAsset("Sprites").height / 8);
	FrameRec = { 0 * SpriteSize.x, 0 * SpriteSize.y, SpriteSize.x, SpriteSize.y };
}

void Player::Update()
{
	CurrentDelay -= GetFrameTime();

	Vector2 OldPosition = Position;
	int goDirection = 0;


	if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		Position.x -= Speed * GetFrameTime();
		goDirection = -1;
	}

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		Position.x += Speed * GetFrameTime();
		goDirection = 1;
	}

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
	{
		Position.y -= Speed * GetFrameTime();
	}

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
	{
		Position.y += Speed * GetFrameTime();
	}


	if (IsKeyDown(KEY_SPACE))
	{
		float HorizontalOffset = Position.x - OldPosition.x;
		float Variation = (HorizontalOffset > 0) ? 1.0f : ((HorizontalOffset < 0) ? -1.f : 0.f);

		Shoot(Variation);
	}

	CurrentFrame = (int)(GetTime() * 16);
	CurrentFrame %= 4;

	unsigned frameSide = (goDirection +1) * 4;

	// Left:	4 frames * -1 +1 
	// Center:	4 frames * 0  +1
	// Right:	4 frames * +1 +1

	FrameRec.x = (frameSide + CurrentFrame) * SpriteSize.x;


	Position.x = Clamp(Position.x, 86, 486);
	Position.y = Clamp(Position.y, 0, 432);

	//DEBUG_COUT("Position y " << Position.y << std::endl);

	if (IsKeyDown(KEY_ONE))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ELockShot);
	}

	if (IsKeyDown(KEY_TWO))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::ESeekShot);
	}

	if (IsKeyDown(KEY_THREE))
	{
		Particles::Get().Create(Vector2Add( Position, Vector2(21, 0) ), BehaviourType::EMultiShot); ///
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
}



void Player::Render()
{
	std::stringstream stream;
	stream << "Position " << std::fixed << std::setprecision(2) <<
		Position.x << " " << Position.y;
	std::string Text = stream.str();

	DrawText(Text.c_str(), 400, 400, 16, WHITE);

	//if(CheckCollisionPointRec(Position, Graphics::Get().GetWindowArea()))
	//	DrawRectangle((int)Position.x, (int)Position.y, 42, 64, WHITE);
	//else
	//	DrawRectangle((int)Position.x, (int)Position.y, 42, 64, BLUE);


	DrawTextureRec(GetAsset("Sprites"), FrameRec, Position, WHITE);  // Draw part of the texture


	//DrawRectanglePro({ Position.x, Position.y, 32, 64}, Vector2Zero(), 0.f, ALPHARED);
	DrawRectanglePro({ Position.x + 8, Position.y + 16, 16, 32}, Vector2Zero(), 0.f, ALPHARED);
}

void Player::Shoot(float displacement)
{
	// https://www.youtube.com/watch?v=QQ3Yub9So2k&t=202s

	if (CurrentDelay > 0.0f)
		return;

	CurrentDelay = DelayTime;

	ShotAngle = Clamp(ShotAngle + displacement * 100, -70, 70) * 0.5f;		// Init Angle
	ShotRotation = Clamp(ShotRotation + displacement, -1.0f, 1.0f) * 0.5f;	// Rotation
	Particles::Get().CreatePlayerShot(Vector2Add(Position, Vector2(40, 24)), ShotAngle, ShotRotation);

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
