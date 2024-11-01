#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"
#include "../Bullet.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct ThrottleMove : public BaseBehaviour
{
	ThrottleMove(Vector2 initPosition, Vector2 direction/* = Vector2(0.f, -1.f)*/, float speed = 350, float acceleration = -10) :
		InitPosition(initPosition), 
		Direction(direction), 
		InitialSpeed(speed),
		Acceleration(acceleration)
	{ ; }

	ThrottleMove(Vector2 initPosition, float rotation = 0.f, float initSpeed = 350, float acceleration = -10, float minSpeed = 50, float maxSpeed = 10000) :
		InitPosition(initPosition), 
		Direction(Vector2Rotate(Vector2(0.f, -1.f), rotation * DEG2RAD)), 
		InitialSpeed(initSpeed),
		MinSpeed(minSpeed),
		MaxSpeed(maxSpeed),
		Acceleration(acceleration)
	{ ; }


	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	virtual void DoDebug();

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 Direction;

	float InitialSpeed;
	float Acceleration;
	float MinSpeed = 50;
	float MaxSpeed = 10000;
	float Rotation = 0.f;

};

