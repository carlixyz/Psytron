#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../Bullet.h"

struct AimMove : public BaseBehaviour
{
	AimMove(Vector2 initPosition, Vector2 targetPosition, float speed = 350) :
		InitPosition(initPosition), 
		TargetPosition(targetPosition), 
		Speed(speed) { ; }


	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	virtual float GetScreenOffset() { return 500.f; }

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 TargetPosition;
	Vector2 Direction = Vector2Zero();
	float Speed;
};

