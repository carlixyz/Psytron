#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Player : public Entity
{
	Player();

	void Update();
	void Render();

	//Vector2 Position{};
	//Vector2 CollisionOffset{};
	//Texture2D Sprite{};
	//Rectangle FrameRec{};

	//float Radius{ 10 };
	float Speed{};

	//unsigned short SpriteFramesCounter{};
	//unsigned short CurrentFrame{};
	//unsigned short FramesSpeed{ 6 };

private:

	void Shoot(float displacement = 0);
	bool bDebug{};
	int CurrentFrame = 0;

	const float DelayTime = 0.05f;// 0.025f; // 0.5f;
	float CurrentDelay = 0.0f;
	float ShotAngle = 0;
	float ShotRotation = 0;

};

