#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../Bullet.h"


#define HALF_PI (PI / 2.0f )

struct LockOnMove : public BaseBehaviour
{
	LockOnMove(Vector2 initPosition, Vector2 targetPosition, float speed = 200, float angSpeed = 10) :
		InitPosition(initPosition), 
		TargetPosition(targetPosition), 
		Speed(speed), 
		AngSpeed(angSpeed)
		{ ; }



	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	virtual float GetScreenOffset() { return 500.f; }

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 TargetPosition;
	Vector2 Direction;

	float Rotation = 0.f;
	float Speed;
	float AngSpeed;
	float AngAcumulation = 0.f;
};

