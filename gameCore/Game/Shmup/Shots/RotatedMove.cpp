#include "RotatedMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "Texture.hpp"
#include "../../Assets.h"
#include "../../../Graphics/Graphics.h"

void RotatedMove::DoInit()
{
	if (Owner != nullptr)
	{
		LifeTime = 5;

		BulletOwner = (Bullet*)Owner;
		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = Vector2Subtract(InitPosition, Vector2(BulletOwner->SpriteScaled.x * 0.5f, 
																	  BulletOwner->SpriteScaled.y * 0.2f) );
		
		DoUpdate();

		///------------------------------------------------------------------------///

		BulletOwner->FrameRec = { 4 * BulletOwner->SpriteSize.x,
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
	};
}

void RotatedMove::DoUpdate()
{
	const float TimeFrame = GetFrameTime();
	LifeTime -= TimeFrame;

	Direction = Vector2Rotate(Direction, RotationOffset * DEG2RAD);
	BulletOwner->Position.x += Direction.x * Speed * TimeFrame;
	BulletOwner->Position.y += Direction.y * Speed * TimeFrame;

	BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
	BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
}

void RotatedMove::DoRender()
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
					   Vector2Angle(Vector2Up(), Direction),
					   WHITE);
	}
}