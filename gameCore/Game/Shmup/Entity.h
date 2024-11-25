#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)

enum class BehaviourType
{
	EPlayerShot = 0,
	EEnemyShot,
	ELinearShot,
	EMultiShot,
	ESpiralShot,
	EMultiSpiralShot,
	ERingShot,
	EStarShot,
	EHanaShot,
	EArchShot,
	ESeekShot,
	ELockShot,
	EExplosion,
	MAX
};


struct Entity;

struct BaseBehaviour
{
	Entity* Owner;
public:
	void SetOwner(Entity* owner)	{ Owner = owner;}

	virtual void DoInit()			{ ; }
	virtual void DoUpdate()			{ ; }
	virtual void DoRender()			{ ; }
	virtual float GetScreenOffset() { return 50.f; }

	float LifeTime = 10.0f;
};

struct Entity
{
	Vector2 Position				{};
	Vector2 SpriteSize				{};
	Vector2 SpriteScaled			{ Vector2Scale(SpriteSize, Graphics::Get().GetFactorArea().x)};

	Rectangle FrameRec =			{ 0.0f, 0.0f, 1.0f, 1.0f };
	Rectangle CollisionRec =		{ 0.0f, 0.0f, 1.0f, 1.0f };

	//raylib::TextureUnmanaged Sprite;

	virtual void Update()			{ ; }
	virtual void Render()			{ ; }
	virtual void DoDebug()			{ ; }

	~Entity() 
	{ 
		if (Behaviour != nullptr) delete Behaviour; 
	}

	inline BaseBehaviour* GetBehaviour() { return Behaviour; }

	inline void SetType(BaseBehaviour* behaviour)
	{
		if (Behaviour != nullptr)
		{
			delete Behaviour;
			Behaviour = nullptr;
		}
		Behaviour = behaviour;
		Behaviour->SetOwner(this);
		Behaviour->DoInit();
	}

protected:
	BaseBehaviour* Behaviour = nullptr;
	bool bDebug{};
};

