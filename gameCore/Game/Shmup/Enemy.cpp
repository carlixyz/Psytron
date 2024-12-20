#include "Player.h"

#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Enemy.h"
#include "Particles.h"
#include "StepAction.h"
#include "reasings.h"
#include "Entity.h"

#define MODULE_FULL 32
#define MODULE_HALF (MODULE_FULL/2)
#define MODULE_FOURTH (MODULE_HALF/2)


Enemy::Enemy(EnemyProperties properties, std::vector<IStepAction*>& steps) :
	Properties(properties),
	Steps(steps)
{
	Position = properties.InitPosition;

	SpriteSize.x = (float)(GetAsset("Sprites").width / MODULE_FOURTH);
	SpriteSize.y = (float)(GetAsset("Sprites").height / MODULE_FOURTH);

	FrameRec = { properties.SpriteIndex.x * SpriteSize.x, 
		properties.SpriteIndex.y * SpriteSize.y, 
		SpriteSize.x, SpriteSize.y };

	FrameOutput = { Position.x, Position.y,
		FrameRec.width * Graphics::Get().GetFactorArea().x,
		FrameRec.height * Graphics::Get().GetFactorArea().y
	};

	Vector2 ColliderSize = { FrameOutput.width * properties.ColliderSizeFactor.x, 
							FrameOutput.height * properties.ColliderSizeFactor.y };

	ColliderOffset = { (FrameOutput.width * 0.5f) - (ColliderSize.x * 0.5f),
						(FrameOutput.height * 0.5f) - (ColliderSize.y * 0.5f) };

	CollisionRec = { Position.x + ColliderOffset.x, Position.y + ColliderOffset.y,
						ColliderSize.x, ColliderSize.y };

	Image imBase = LoadImageFromTexture(GetAsset("Sprites"));
	Image imCopy = ImageCopy(imBase);
	ImageColorBrightness(&imCopy, +100);
	SpriteFlash.Load(imCopy);
	LoadTextureFromImage(imCopy);

	Steps = std::move(steps);

	if (!Steps.empty())
		SetCurrentAction(Steps[0]);

	Active = true;
}

/*
Enemy::Enemy(Vector2 initPosition, std::vector<IStepAction*>& steps, Vector2 spriteIndex, Vector2 colliderSize)
{
	Position = initPosition;

	SpriteSize.x = (float)(GetAsset("Sprites").width / MODULE_FOURTH);
	SpriteSize.y = (float)(GetAsset("Sprites").height / MODULE_FOURTH);

	FrameRec = { spriteIndex.x * SpriteSize.x, spriteIndex.y * SpriteSize.y, SpriteSize.x, SpriteSize.y }; 

	FrameOutput = { Position.x, Position.y,
		FrameRec.width * Graphics::Get().GetFactorArea().x,
		FrameRec.height * Graphics::Get().GetFactorArea().y
	};

	Vector2 ColliderSize = { FrameOutput.width * colliderSize.x, FrameOutput.height * colliderSize.y };

	ColliderOffset = { (FrameOutput.width * 0.5f) - (ColliderSize.x * 0.5f),
						(FrameOutput.height * 0.5f) - (ColliderSize.y * 0.5f) };

	CollisionRec = {
		Position.x + ColliderOffset.x, Position.y + ColliderOffset.y,
		ColliderSize.x, ColliderSize.y
	};

	Image imBase = LoadImageFromTexture(GetAsset("Sprites"));
	Image imCopy = ImageCopy(imBase);
	ImageColorBrightness(&imCopy, +100);
	SpriteFlash.Load(imCopy);
	LoadTextureFromImage(imCopy);

	Steps = std::move(steps);

	 if (!Steps.empty())
			SetCurrentAction(Steps[0]);

	Active = true;
}
*/

Enemy::~Enemy()
{
	for (auto steps : Steps)
	{
		delete steps;
	}
	Steps.clear();

	Behaviour = nullptr;
}

void Enemy::Update()
{
	if (DoDamageFlash) 
		UpdateDamage();

	if (Behaviour != nullptr)
	{
		Behaviour->DoUpdate();
	}

	UpdateRectangles();
}

void Enemy::ApplyDamage()
{
	DoDamageFlash = true;
	Properties.Life -= 5;

	if (Properties.Life <= 0)
	{
		const Vector2 explotion(Position.x + FrameOutput.width * 0.5f, Position.y + FrameOutput.height * 0.5f);
		Particles::Get().Create(explotion, BehaviourType::EExplotion);
 		Active = false;
	}
}

void Enemy::UpdateDamage()
{
	if (!DoDamageFlash)
		return;

	CurrentFlashTime -= GetFrameTime();
	ShowSpriteFlash = EaseSineInOut(CurrentFlashTime, 0.f, 1.0f, 0.05f) > 0.5f;
	///Size.x = EaseLinearIn(CurrentTime, StartValue, EndValue - StartValue, FullCycleTime);

	if (CurrentFlashTime < 0)
	{
		DoDamageFlash = false;
		ShowSpriteFlash = false;
		CurrentFlashTime = totalFlashTime;
	}
}

void Enemy::UpdateRectangles()
{
	FrameOutput.x = Position.x; // = Clamp(Position.x, LeftMargin, RightMargin);
	FrameOutput.y = Position.y; // = Clamp(Position.y, 0, BottomMargin);

	CollisionRec.x = Position.x + ColliderOffset.x; 
	CollisionRec.y = Position.y + ColliderOffset.y;
}

void Enemy::Render()
{
	DrawRectanglePro(CollisionRec, Vector2Zero(), 0.f, ALPHARED);

	DrawTexturePro(ShowSpriteFlash ? SpriteFlash : GetAsset("Sprites"), 
				   FrameRec, 
				   FrameOutput, 
				   Vector2Zero(), 
				   0.f, 
				   WHITE);  // Draw part of the texture


	if (Behaviour != nullptr)
		Behaviour->DoDebug();
}

void Enemy::ProcessStep()
{
	if (Steps.empty())
		return;

	IStepAction* currentStep = Steps[CurrentStepIndex];

	if (currentStep && currentStep->Done)
	{
		currentStep->Done = false;
		CurrentStepIndex++;
		CurrentStepIndex %= Steps.size(); // TotalSteps

		if (IStepAction* nextStep = Steps[CurrentStepIndex])
		{
			SetCurrentAction(nextStep);
		}
	}
}

void Enemy::SetCurrentAction(IStepAction* action)
{
	Behaviour = action;
	Behaviour->SetOwner(this);
	action->EnemyOwner = (Enemy*)this;
	//Behaviour->DoInit();
}
