#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../Bullet.h"

struct PetalMove : public BaseBehaviour
{
	PetalMove(Vector2 initPosition, Vector2 direction/* = Vector2(0.f, -1.f)*/, float speed = 350, float rotationOffset = 0.0f, float lifeTime = 5.0f) :
		InitPosition(initPosition), 
		Direction(direction), 
		Speed(speed),
		RotationOffset(rotationOffset)
	{ ; }

	PetalMove(Vector2 initPosition, float rotation = 0.f, float speed = 350, float rotationOffset = 0.0f, float lifeTime = 5.0f) :
		InitPosition(initPosition), 
		Direction(Vector2Rotate(Vector2(0.f, -1.f), rotation * DEG2RAD)), 
		Speed(speed),
		RotationOffset(rotationOffset)
	{ ; }


	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	virtual float GetScreenOffset() { return 40.f; }


protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 Direction;

	float Speed;
	float RotationOffset = 0.0f;

};

