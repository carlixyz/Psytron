#include "AimMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"
#include "../../../Audio/Audio.h"


void AimMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;

		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = InitPosition;

		Vector2 Orientation = Vector2Subtract(TargetPosition, BulletOwner->Position);
		Direction = Vector2Normalize(Orientation);

		BulletOwner->FrameRec = {	2 * BulletOwner->SpriteSize.x,
									2 * BulletOwner->SpriteSize.y,
									BulletOwner->SpriteSize.x,
									BulletOwner->SpriteSize.y };

		///----------------------------------------------------------------- ///

		const float CollisionSizeFactor = 0.2f;
		const float ColliderSide = BulletOwner->SpriteScaled.x * CollisionSizeFactor;

		BulletOwner->CollisionRec = {
			BulletOwner->Position.x - ColliderSide * 0.5f,
			BulletOwner->Position.y - ColliderSide * 0.5f,
			ColliderSide, ColliderSide
		};

	}
}

void AimMove::DoUpdate()
{
	BulletOwner->Position.x += Direction.x * Speed * GetFrameTime() * GetShotSpeedFactor();
	BulletOwner->Position.y += Direction.y * Speed * GetFrameTime() * GetShotSpeedFactor();

	BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
	BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
}

void AimMove::DoRender()
{
		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
					   },
					   Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
					   0.f,
					   WHITE);
}