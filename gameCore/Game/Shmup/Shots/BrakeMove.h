#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"
#include "../Bullet.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct BrakedMove : public BaseBehaviour
{
	BrakedMove(Vector2 initPosition, Vector2 direction, float speed, BaseBehaviour* wrapBehaviour, float brake = 1.0f) :
		InitPosition(initPosition), 
		Direction(direction), 
		Speed(speed),
		WrappedBehaviour(wrapBehaviour),
		Brake(brake)
	{ ; }

	BrakedMove(Vector2 initPosition, float rotation, float speed, BaseBehaviour* wrapBehaviour, float brake = 0.25f) :
		InitPosition(initPosition), 
		Direction(Vector2Rotate(Vector2(0.f, -1.f), rotation * DEG2RAD)), 
		Speed(speed),
		WrappedBehaviour(wrapBehaviour),
		Brake(brake)
	{ ;	}

	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();
	~BrakedMove()
	{
		if (WrappedBehaviour != nullptr)
			delete WrappedBehaviour;
	}

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 Direction;
	
	float Speed = 10000;
	float Brake = 1.0f;

	BaseBehaviour* WrappedBehaviour;
};

