#include "RotatedMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "Texture.hpp"
#include "../../Assets.h"
//#include <../../../Utility/Utils.h>


void RotatedMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
		BulletOwner->Position = InitPosition;
		LifeTime = 5;

		

		DoUpdate();

		BulletOwner->FrameRec = { 4 * BulletOwner->SpriteSize.x,
									2 * BulletOwner->SpriteSize.y,
										BulletOwner->SpriteSize.x,
										BulletOwner->SpriteSize.y };

		//float SpriteWidth = (float)(GetAsset("Sprites").width / 8);
		//float SpriteHeight = (float)(GetAsset("Sprites").height / 8);

		//BulletOwner->FrameRec = { 4 * BulletOwner->SpriteSize.x, 
		//							2 * BulletOwner->SpriteSize.y,
		//								BulletOwner->SpriteSize.x, 
		//								BulletOwner->SpriteSize.y };
		 
		//Direction = Vector2Rotate(Direction, 180 * DEG2RAD);
	}
}

void RotatedMove::DoUpdate()
{
	const float TimeFrame = GetFrameTime();
	LifeTime -= TimeFrame;

	Direction = Vector2Rotate(Direction, RotationOffset * DEG2RAD);
	//Speed = 10;
	BulletOwner->Position.x += Direction.x * Speed * TimeFrame;
	BulletOwner->Position.y += Direction.y * Speed * TimeFrame;
}

void RotatedMove::DoRender()
{
	if (BulletOwner != nullptr)
	{

		//DrawTextureRec(GetAsset("Sprites"), BulletOwner->FrameRec, BulletOwner->Position, WHITE);  // Draw part of the texture
		DrawTexturePro(GetAsset("Sprites"), BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteSize.x, BulletOwner->SpriteSize.y
					   },
					   //Vector2Scale(BulletOwner->SpriteSize, 0.5f),
					   BulletOwner->SpriteSize,
					   Vector2Angle(Vector2Up(), Direction),
					   WHITE);
	}
}