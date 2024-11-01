#include "PetalMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "Texture.hpp"
#include "../../Assets.h"


void PetalMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
		BulletOwner->Position = InitPosition;

		//BulletOwner->FrameRec = {  0 * BulletOwner->SpriteSize.x,
		BulletOwner->FrameRec = {  1 * BulletOwner->SpriteSize.x,
								2 * BulletOwner->SpriteSize.y,
								BulletOwner->SpriteSize.x, 
								BulletOwner->SpriteSize.y };

		//Direction = Vector2Rotate(Direction, 180 * DEG2RAD);
	}
}

void PetalMove::DoUpdate()
{
	const float TimeFrame = GetFrameTime();
	LifeTime -= TimeFrame;

	Direction = Vector2Rotate(Direction, RotationOffset * DEG2RAD);

	BulletOwner->Position.x += Direction.x * Speed * TimeFrame;
	BulletOwner->Position.y += Direction.y * Speed * TimeFrame;
}

void PetalMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawTextureRec(GetAsset("Sprites"), BulletOwner->FrameRec, BulletOwner->Position, WHITE);  // Draw part of the texture

		DrawTexturePro(GetAsset("Sprites"), BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteSize.x, BulletOwner->SpriteSize.y
					   },
					   Vector2Scale(BulletOwner->SpriteSize, 0.5f),
					   std::atan2(-Direction.x, Direction.y) * RAD2DEG,
					   WHITE);

	}
}