#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../Bullet.h"

struct ChaseMove : public BaseBehaviour
{
	ChaseMove(Vector2 initPosition, Vector2 targetPosition, float speed = 250, bool continousTracking = false) :
		InitPosition(initPosition), 
		TargetPosition(targetPosition), 
		Speed(speed), 
		Tracking(continousTracking) { ; }


	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	virtual float GetScreenOffset() { return 500.f; }

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 TargetPosition;
	Vector2 Direction;
	float Speed;
	bool Tracking;
};

