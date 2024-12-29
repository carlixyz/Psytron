#pragma once

#include <raylib-cpp.hpp>
#include "../../Graphics/Graphics.h"
#include "Enemy.h"

struct IStepAction;



struct Boss : public Enemy
{
	Boss() = default;
	Boss(EnemyProperties properties, std::vector<IStepAction*>& steps) : Enemy(properties, steps) {}
	Boss(const Enemy& other);
	//~Boss();

	Enemy* Clone() const { return new Boss(*this); }

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual bool ApplyDamage() override;

protected:
	friend struct IStepAction;
	
	void UpdateShieldState();

	bool IsShieldEnabled	= false;
	int CurrentCol			= 0;
	int FramesCounter		= 0;
	int Speed				= 2;

	float StateDelay		= 10.f;
	float StateChange		= StateDelay;
	bool DoingTransition	= false;

};


