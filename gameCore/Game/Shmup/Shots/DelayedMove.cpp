#include "DelayedMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"

void DelayedMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;
 		BulletOwner->Position = InitPosition;

		WrappedBehaviour->Owner = Owner;
		WrappedBehaviour->DoInit();
	}
}

void DelayedMove::DoUpdate()
{
	if (CurrentDelay > 0.0f)
	{
		BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
		BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();
		CurrentDelay -= GetFrameTime();

		BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
		BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
	}
	else
	{
		WrappedBehaviour->DoUpdate();
	}
}

void DelayedMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, BLUE);
	}
}