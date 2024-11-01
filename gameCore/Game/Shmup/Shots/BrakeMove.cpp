#include "BrakeMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
//#include <../../../Utility/Utils.h>

void BrakedMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
 		BulletOwner->Position = InitPosition;

		WrappedBehaviour->Owner = Owner;
		WrappedBehaviour->DoInit();

		BulletOwner->FrameRec = { 3 * BulletOwner->SpriteSize.x,
							2 * BulletOwner->SpriteSize.y,
								BulletOwner->SpriteSize.x,
								BulletOwner->SpriteSize.y };

	}
}

void BrakedMove::DoUpdate()
{
	//Direction = Vector2Rotate(Direction, 2 * DEG2RAD);

	if (Speed > 0.0f)
	{
		BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
		BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();
		Speed -= Brake;
	}
	else
	{
		WrappedBehaviour->DoUpdate();
	}
}

void BrakedMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);
		//DEBUG_COUT("Position y " << bulletOwner->Position.y << std::endl);

		//static const float VisualRotationScaling = 10;

		const float angleVar = std::atan2(-Direction.x, Direction.y);

		DrawTexturePro(GetAsset("Sprites"), BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteSize.x, BulletOwner->SpriteSize.y
					   },
					   //BulletOwner->SpriteSize,
					   Vector2Scale(BulletOwner->SpriteSize, 0.5f),
					   //Vector2Angle(Vector2Up(), Direction) * VisualRotationScaling,
					   angleVar * RAD2DEG,
					   WHITE);
	}
}