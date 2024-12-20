#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Entity.h"


struct IStepAction;

struct EnemyProperties
{
	float Life					= 100;
	float Damage				= 5; 

	Vector2 InitPosition		= Vector2One();			// Entity start position
	Vector2 SpriteIndex			= Vector2One();			// Which is the sprite image to show 
	Vector2 ColliderSizeFactor	= Vector2(.35f, .35f);	// How much of the collider covers the Sprite size 
};

struct Enemy : public Entity
{
	Enemy(EnemyProperties properties, std::vector<IStepAction*>& steps);
	~Enemy();

	virtual void Update() override;
	virtual void Render() override;

	inline const Vector2 GetShotOffset() { return ShotInitialOffset;}
	void ApplyDamage();

	EnemyProperties Properties;
	const float LeftMargin		= 86 * Graphics::Get().GetFactorArea().x;
	const float RightMargin		= 486 * Graphics::Get().GetFactorArea().x;
	const float BottomMargin	= 432 * Graphics::Get().GetFactorArea().y;

protected:
	friend struct IStepAction;

	void UpdateDamage();
	void UpdateRectangles();

	Rectangle FrameOutput{};
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

