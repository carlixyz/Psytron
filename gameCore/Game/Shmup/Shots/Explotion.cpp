#include "Explotion.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../Assets.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Utility/Utils.h"

#define MODULE_FULL 32
#define MODULE_HALF (MODULE_FULL/2)
#define MODULE_FOURTH (MODULE_HALF/2)

void Explotion::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;

		BulletOwner->Position = InitPosition;

		BulletOwner->FrameRec = { 0 * BulletOwner->SpriteSize.x,
									4 * BulletOwner->SpriteSize.y,
									BulletOwner->SpriteSize.x,
									BulletOwner->SpriteSize.y };

		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y * Size);

		CurrentCol = 0;
		framesCounter = 0;
		CurrentRow = 0;
		LifeTime = 1;
	}
}

void Explotion::DoUpdate()
{
	framesCounter++;

	if (framesCounter >= (60 / Speed))
	{
		framesCounter = 0;
		CurrentCol++;

		if (CurrentCol > 7)
		{
			CurrentCol = 0;
			if (++CurrentRow > 1)
			{
				framesCounter = 0;
				CurrentRow = 0;
				LifeTime = 0;
				Owner->Active = false;
			}
		}
	}

	Owner->FrameRec.x = (CurrentCol) * Owner-> SpriteSize.x;
	Owner->FrameRec.y = (4 + (CurrentRow)) * Owner->SpriteSize.y;
}

void Explotion::DoRender()
{
	DrawTexturePro(GetAsset("Sprites"),
		BulletOwner->FrameRec,
		{
			BulletOwner->Position.x, BulletOwner->Position.y,
			BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
		},
		Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
		Angle,
		WHITE);
}