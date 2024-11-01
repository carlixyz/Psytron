#include "DelayedMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
//#include <../../../Utility/Utils.h>

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
	//Direction = Vector2Rotate(Direction, 2 * DEG2RAD);

	if (CurrentDelay > 0.0f)
	{
		BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
		BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();
		CurrentDelay -= GetFrameTime();
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
		DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);
		//DEBUG_COUT("Position y " << bulletOwner->Position.y << std::endl);
	}
}