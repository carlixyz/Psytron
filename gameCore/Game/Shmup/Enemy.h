#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Enemy : public Entity
{
	void Update();
	void Render();

	////Vector2 Location{};
	////Vector2 CollisionOffset{};
	////Texture2D Sprite{};
	////Rectangle FrameRec{};

	//float Radius{ 10 };
	//float Speed{};

	//unsigned short SpriteFramesCounter{};
	//unsigned short CurrentFrame{};
	//unsigned short FramesSpeed{ 6 };

private:
	bool bDebug{};
};

