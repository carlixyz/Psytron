#include "ThrottleMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"

void ThrottleMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = InitPosition;


		BulletOwner->FrameRec = { 7 * BulletOwner->SpriteSize.x,
								2 * BulletOwner->SpriteSize.y,
								BulletOwner->SpriteSize.x,
								BulletOwner->SpriteSize.y };

		const float CollisionSizeFactor = 0.2f;
		const float ColliderSide = BulletOwner->SpriteScaled.x * CollisionSizeFactor;

		BulletOwner->CollisionRec = {
			BulletOwner->Position.x - ColliderSide * 0.5f,
			BulletOwner->Position.y - ColliderSide * 0.5f,
			ColliderSide, ColliderSide
		};
	}
}

void ThrottleMove::DoUpdate()
{
	const float timeFrame = GetFrameTime();
	Rotation += timeFrame * MaxSpeed;

	InitialSpeed += Acceleration;
	InitialSpeed = Clamp(InitialSpeed, MinSpeed, MaxSpeed);

	BulletOwner->Position.x += Direction.x * InitialSpeed * timeFrame;
	BulletOwner->Position.y += Direction.y * InitialSpeed * timeFrame;

	BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
	BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
}

void ThrottleMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);
		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
					   },
					   Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
					   std::atan2(-Direction.x, Direction.y)* RAD2DEG,
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