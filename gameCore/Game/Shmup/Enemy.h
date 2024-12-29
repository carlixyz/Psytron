#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"

struct IStepAction;

#define SPRITES_PER_ROW 8


struct EnemyProperties
{
	Vector2 InitPosition		= Vector2One();			// Entity start position
	Vector2 SpriteIndex			= Vector2One();			// Which is the sprite image to show 

	float Life					= 100;
	float Damage				= 5; 

	Vector2 ColliderSizeFactor	= Vector2(.35f, .35f);	// How much of the collider covers the Sprite size

	unsigned StartIndex			= 0;
	unsigned SpritesCount		= SPRITES_PER_ROW;
	std::string ResourceName	= "Sprites";
};

struct Enemy : public Entity
{
	Enemy() = default;
	Enemy(EnemyProperties properties, std::vector<IStepAction*>& steps);
	Enemy(const Enemy& other);/// { /* copy construction from rhs*/ }
	~Enemy();

	Enemy* Clone() const { return new Enemy(*this); }

	virtual void Init();
	virtual void Update() override;
	virtual void Render() override;

	virtual bool ApplyDamage();
	inline const Vector2 GetShotOffset() { return ShotInitialOffset;}
	inline const Vector2 GetTargetCenter() { return TargetCenter;}

	EnemyProperties Properties;

protected:
	friend struct IStepAction;
	friend struct Boss;

	void UpdateDamage();
	void UpdateRectangles();

	Rectangle FrameOutput{};
	Vector2 TargetCenter{};
	Vector2 ColliderOffset{};
	Vector2 ShotInitialOffset = Vector2(32 * Graphics::Get().GetFactorArea().x, 
										32 * Graphics::Get().GetFactorArea().y);

	/// Process Sprite Flash when entity get damaged
	bool DoDamageFlash			= false;
	bool ShowSpriteFlash		= false;
	float CurrentFlashTime		= 0.0f;
	float totalFlashTime		= 0.25f;
	raylib::Texture SpriteFlash;

	/// Process Current & Next Action behavior
	unsigned CurrentStepIndex = 0;
	std::vector<IStepAction*> Steps;
	void ProcessStep();
	void SetCurrentAction(IStepAction* action);
};

#undef SPRITES_PER_COLUMN

