#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../Bullet.h"

struct Explotion : public BaseBehaviour
{
	Explotion(Vector2 initPosition, float size = 1.0f, float angle = 0.f, int speed = 24) :
		InitPosition(initPosition), 
		Size(size),
		Angle(angle),
		Speed(speed) { ; }

	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;

	int Speed;		// Number of spritesheet frames shown by second (I.Ex 16 ~ 24) 
	float Size;
	float Angle;

	int CurrentCol		= 0;
	int CurrentRow		= 0;
	int framesCounter	= 0;
};

