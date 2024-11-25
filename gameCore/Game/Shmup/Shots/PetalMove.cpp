#include "PetalMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "Texture.hpp"
#include "../../Assets.h"
#include "../../../Graphics/Graphics.h"

void PetalMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;

		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = InitPosition;
		//BulletOwner->Position = Vector2Subtract(InitPosition, Vector2Scale(BulletOwner->SpriteScaled, 0.5f));

		BulletOwner->FrameRec = {  1 * BulletOwner->SpriteSize.x,
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

void PetalMove::DoUpdate()
{
	const float TimeFrame = GetFrameTime();
	LifeTime -= TimeFrame;

	Direction = Vector2Rotate(Direction, RotationOffset * DEG2RAD);

	BulletOwner->Position.x += Direction.x * Speed * TimeFrame;
	BulletOwner->Position.y += Direction.y * Speed * TimeFrame;

	BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
	BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
}

void PetalMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawTextureRec(GetAsset("Sprites"), BulletOwner->FrameRec, BulletOwner->Position, WHITE);  // Draw part of the texture

		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
					   },
					   Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
					   std::atan2(-Direction.x, Direction.y) * RAD2DEG,
					   WHITE);
	}
}