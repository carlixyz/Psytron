#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"
#include "../Assets.h"

#define ALPHARED CLITERAL(Color){ 230, 41, 55, 128 }     // Red

struct Player;

struct Bullet : public Entity
{
	Bullet() : Damage(10), IsHit(false), Active(false)
	{
		SpriteSize.x = (float)(GetAsset("Sprites").width / 8);
		SpriteSize.y = (float)(GetAsset("Sprites").height / 8);
	}

	virtual inline void Update() 
	{ 
		if (Behaviour != nullptr)
		{
			Behaviour->DoUpdate(); 

			const float& ScreenOffset = Behaviour->GetScreenOffset(); // usually 0 but 500 for PetalMove (Player Shot)

			Active = Behaviour->LifeTime > 0 &&
				(Position.y > -ScreenOffset && Position.y < Graphics::Get().GetWindowArea().height + ScreenOffset) &&
				(Position.x > -ScreenOffset && Position.x < Graphics::Get().GetWindowArea().width + ScreenOffset);
		}
	}

	virtual inline void Render() 
	{ 
		if (Behaviour != nullptr)
		{
			Behaviour->DoRender();

			//if (bDebug)
				//DoDebug();
		}
	}

	virtual inline void DoDebug()
	{
		//const Rectangle rect = { BulletOwner->Position.x - (BulletOwner->SpriteSize.x * 0.25f), 
		//						BulletOwner->Position.y - (BulletOwner->SpriteSize.x * 0.25f),
		//						BulletOwner->SpriteSize.x * 0.5f, BulletOwner->SpriteSize.y * 0.5f };
		//DrawRectangleRec(rect, ALPHARED);

		const float CollisionSizeFactor = 0.25f;
		const Vector2 CollisionSize = Vector2Scale(SpriteSize, CollisionSizeFactor);
		DrawRectangleV(Vector2Subtract(Position, Vector2Scale(SpriteSize, CollisionSizeFactor * 0.5f)),
					   CollisionSize, ALPHARED);

		//DrawRectangleV(Vector2Subtract(Position, Vector2Scale(SpriteSize, 0.125f)),
		//			   Vector2Scale(SpriteSize, 0.25f), ALPHARED);

		DrawCircle( (int)Position.x, (int)Position.y, 2, MAGENTA);

		//DrawCircle(BulletOwner->Position.x, BulletOwner->Position.y, 10, ALPHARED);
	}


	//Entity* Target = nullptr;

	float Radius{ 5 };
	unsigned short Damage;

	bool IsHit;
	bool Active;

//private:
	//bool bDebug{};
};

