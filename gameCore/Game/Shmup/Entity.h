#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"

//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)


enum class EntityType
{
	EPlayerShot,
	EEnemyShot,
	EItem,
	EOther,
	MAX
};

enum class BehaviourType
{
	ENone = 0,
	EExplotion,
	ELinearShot,
	EAimShot,			// Simple one time Aiming target shot
	ESeekShot,			// fast one time Aiming target shot
	ELockShot,			// Player Lock on shot
	EMultiShot,			// 8 shots in 8 directions
	ESpiralShot,
	EMultiSpiralShot,
	ERingShot,			// Multiple 30 shots
	EStarShot,			// Multiple 30 shape star shot
	EHanaShot,
	EArchShot,
	MAX
};


struct Entity;

/// Common customizable behavior interface for anything Entity inherited 
struct BaseBehaviour
{
	Entity* Owner;
public:
	void SetOwner(Entity* owner)	{ Owner = owner;}

	virtual void DoInit()			{ ; }
	virtual void DoUpdate()			{ ; }
	virtual void DoRender()			{ ; }
	virtual void DoDebug()			{ ; }
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

	bool IsHit = false;
	bool Active = false;

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

