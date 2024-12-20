#include "BrakeMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"

void BrakedMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;

		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = Vector2Subtract(InitPosition, Vector2Scale(BulletOwner->SpriteScaled, 0.5f));

		WrappedBehaviour->Owner = Owner;
		WrappedBehaviour->DoInit();

		BulletOwner->FrameRec = { 3 * BulletOwner->SpriteSize.x,
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

void BrakedMove::DoUpdate()
{
	if (Speed > 0.0f)
	{
		BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
		BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();

		Speed -= Brake;

		BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
		BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
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

		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
					   },
					   Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
					   angleVar* RAD2DEG,
					   WHITE);
	}
}