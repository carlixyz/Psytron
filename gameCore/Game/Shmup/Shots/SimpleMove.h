#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"
#include "../Bullet.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct SimpleMove : public BaseBehaviour
{
	SimpleMove(Vector2 initPosition, Vector2 direction/* = Vector2(0.f, -1.f)*/, float speed = 350) :
		InitPosition(initPosition), 
		Direction(direction), 
		Speed(speed) { ; }

	SimpleMove(Vector2 initPosition, float rotation = 0.f, float speed = 350) :
		InitPosition(initPosition), 
		Direction(Vector2Rotate(Vector2(0.f, -1.f), rotation * DEG2RAD)), 
		Speed(speed) { ; }


	virtual void DoInit();
	virtual void DoUpdate();
	virtual void DoRender();

protected:
	Bullet* BulletOwner = nullptr;
	Vector2 InitPosition;
	Vector2 Direction;
	float Speed;

};

