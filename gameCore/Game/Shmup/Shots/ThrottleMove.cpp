#include "ThrottleMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
//#include <../../../Utility/Utils.h>

void ThrottleMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
 		BulletOwner->Position = InitPosition;
		//Direction = Vector2Rotate(Direction, 180 * DEG2RAD);

		BulletOwner->FrameRec = { 7 * BulletOwner->SpriteSize.x,
							2 * BulletOwner->SpriteSize.y,
							BulletOwner->SpriteSize.x,
							BulletOwner->SpriteSize.y };
	}
}

void ThrottleMove::DoUpdate()
{
	//Direction = Vector2Rotate(Direction, 2 * DEG2RAD);
	const float timeFrame = GetFrameTime();
	Rotation += timeFrame * MaxSpeed;

	InitialSpeed += Acceleration;
	InitialSpeed = Clamp(InitialSpeed, MinSpeed, MaxSpeed);

	BulletOwner->Position.x += Direction.x * InitialSpeed * timeFrame;
	BulletOwner->Position.y += Direction.y * InitialSpeed * timeFrame;
}

void ThrottleMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);
		//DEBUG_COUT("Position y " << bulletOwner->Position.y << std::endl);
		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteSize.x, BulletOwner->SpriteSize.y
					   },
					   Vector2Scale(BulletOwner->SpriteSize, 0.5f),
					   //Rotation* RAD2DEG,
					   std::atan2(-Direction.x, Direction.y) * RAD2DEG,
					   WHITE);
	}
}

void ThrottleMove::DoDebug()
{
	if (BulletOwner != nullptr)
	{
		const Rectangle rect = { BulletOwner->Position.x, BulletOwner->Position.y, BulletOwner->SpriteSize.x * 0.5f, BulletOwner->SpriteSize.y * 0.5f };
		DrawRectanglePro(rect,
						 Vector2(BulletOwner->SpriteSize.x * 0.75f, BulletOwner->SpriteSize.y * 0.75f),	 //BulletOwner->SpriteSize,
						 0.f,														//Vector2Angle(Vector2Up(), Direction) * RAD2DEG,
						 ALPHARED);
	}
}