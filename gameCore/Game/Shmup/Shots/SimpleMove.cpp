#include "SimpleMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../Assets.h"

//#include <../../../Utility/Utils.h>

void SimpleMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
 		BulletOwner->Position = InitPosition;
		//BulletOwner->Sprite 
		//Direction = Vector2Rotate(Direction, 180 * DEG2RAD);
		BulletOwner->FrameRec = { 0 * BulletOwner->SpriteSize.x,
									2 * BulletOwner->SpriteSize.y,
									BulletOwner->SpriteSize.x,
									BulletOwner->SpriteSize.y };
	}
}

void SimpleMove::DoUpdate()
{
	//Direction = Vector2Rotate(Direction, 2 * DEG2RAD);

	BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
	BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();
}

void SimpleMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);
		//DEBUG_COUT("Position y " << bulletOwner->Position.y << std::endl);

		DrawTextureRec(GetAsset("Sprites"), BulletOwner->FrameRec, 
					   Vector2Subtract(BulletOwner->Position, Vector2Scale(BulletOwner->SpriteSize, 0.5f)), WHITE);
	}
}