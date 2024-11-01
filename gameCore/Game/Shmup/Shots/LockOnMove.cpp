#include "LockOnMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Utility/Utils.h"
#include "../../Assets.h"
#include <iomanip>
#include <string>

void LockOnMove::DoInit()
{
	if (Owner != nullptr)
	{
		LifeTime = 3.f;
		BulletOwner = (Bullet*)Owner;
		BulletOwner->Position = Vector2Subtract(InitPosition, Vector2(32, 32));
		Direction = Vector2Up(); //Direction = Vector2Rotate(Vector2Up(), Rotation * DEG2RAD);

		DoUpdate();

		BulletOwner->FrameRec = { 5 * BulletOwner->SpriteSize.x,
									2 * BulletOwner->SpriteSize.y,
										BulletOwner->SpriteSize.x,
										BulletOwner->SpriteSize.y };
	}
}

void LockOnMove::DoUpdate()
{
	/// Look at Me!
	/// Direction = Vector2Subtract(TargetPosition, BulletOwner->Position);
	/// Rotation = std::atan2(-Direction.x, Direction.y);
	/// 
	/// 
	const float timeLapse = GetFrameTime();

	LifeTime -= timeLapse;

	Vector2 Distance = Vector2Subtract(TargetPosition, BulletOwner->Position);

	// Calculate the forward vector : The forward vector is obtained by subtracting the object’s position from the target position, resulting in a vector that points from the object toward the target.
	Vector2 Forward = Vector2Subtract(Vector2Add(BulletOwner->Position, Direction), BulletOwner->Position);

	// Normalize the forward vector : Normalizing the forward vector ensures that it has a length of 1, preserving the object’s scale and preventing undesired distortions.
	Distance = Vector2Normalize(Distance);
	Forward = Vector2Normalize(Direction);

	///----------------------------------------------------------------- ///
	AngAcumulation = Lerp(AngAcumulation, AngSpeed, timeLapse);
	Direction = Vector2Lerp(Forward, Distance, AngAcumulation * timeLapse);
	//Direction = Vector2Lerp(Forward, Distance, AngSpeed * timeLapse);

	Rotation = std::atan2(-Direction.x, Direction.y);

	if (BulletOwner)
	{
		//Speed = 30;
		BulletOwner->Position.x += Direction.x * Speed * timeLapse;
		BulletOwner->Position.y += Direction.y * Speed * timeLapse;
	}
	///--------------------------------------------------------------------
}

void LockOnMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteSize.x, BulletOwner->SpriteSize.y
					   },
					   Vector2Scale(BulletOwner->SpriteSize, 0.5f), //BulletOwner->SpriteSize,
					   Rotation * RAD2DEG,
					   WHITE);
	}
}