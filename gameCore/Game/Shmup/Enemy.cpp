#include "Enemy.h"

#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Player.h"
#include "Particles.h"
#include "StepAction.h"
#include "reasings.h"
#include "Entity.h"
#include "../../Audio/Audio.h"




Enemy::Enemy(EnemyProperties properties, std::vector<IStepAction*>& steps) :
	Properties(properties)		
	//Steps(steps)					// This makes a shallow copy of list but with the same items in memory
	//Steps(std::move(steps))		// This doesn't share the steps list with anyone else
{
	for (const auto& step : steps)	// This Clones a deep copy of everything (with own new space in memory)
	{
		auto stepClone = step->Clone();
		stepClone->SetOwner(this);
		stepClone->EnemyOwner = (Enemy*)this;
		Steps.push_back(stepClone);
	}

	Init();

	Active = true;
}


Enemy::Enemy(const Enemy& other) : Properties(other.Properties)
{
	for (const auto& step : other.Steps)
	{
		auto stepClone = step->Clone();
		stepClone->SetOwner(this);
		stepClone->EnemyOwner = (Enemy*)this;
		Steps.push_back(stepClone);
	}

	Init();

	Active = true;
}


void Enemy::Init()
{
	CurrentStepIndex = Properties.StartIndex;
	Position = Properties.InitPosition;

	SpriteSize.x = (float)(GetAsset(Properties.ResourceName).width / Properties.SpritesCount);
	SpriteSize.y = (float)(GetAsset(Properties.ResourceName).height / Properties.SpritesCount);

	FrameRec = { Properties.SpriteIndex.x * SpriteSize.x,
		Properties.SpriteIndex.y * SpriteSize.y,
		SpriteSize.x, SpriteSize.y };

	FrameOutput = { Position.x, Position.y,
		FrameRec.width * Graphics::Get().GetFactorArea().x,
		FrameRec.height * Graphics::Get().GetFactorArea().y
	};

	Vector2 ColliderSize = { FrameOutput.width * Properties.ColliderSizeFactor.x,
		FrameOutput.height * Properties.ColliderSizeFactor.y };

	ColliderOffset = { (FrameOutput.width * 0.5f) - (ColliderSize.x * 0.5f),
		(FrameOutput.height * 0.5f) - (ColliderSize.y * 0.5f) };

	CollisionRec = { Position.x + ColliderOffset.x, Position.y + ColliderOffset.y,
		ColliderSize.x, ColliderSize.y };

	Image imBase = LoadImageFromTexture(GetAsset(Properties.ResourceName));
	Image imCopy = ImageCopy(imBase);
	ImageColorBrightness(&imCopy, +100);
	SpriteFlash.Load(imCopy);
	LoadTextureFromImage(imCopy);
	CurrentFlashTime = totalFlashTime;

	if (!Steps.empty())
		SetCurrentAction(Steps[CurrentStepIndex]);
}

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
		Behaviour->DoUpdate();

	UpdateRectangles();
}

bool Enemy::ApplyDamage()
{
	DoDamageFlash = true;
	Properties.Life -= Properties.Damage;

	if (Properties.Life <= 0)
	{
		const Vector2 explotion(Position.x + FrameOutput.width * 0.5f, Position.y + FrameOutput.height * 0.5f);
		Particles::Get().Create(explotion, BehaviourType::EExplotion);
		Audio::Get().PlaySound("Data/Sound/Explode.wav");

		Game::Get().AddScore();

		return false;
	}

	return true;
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

	TargetCenter.x = FrameOutput.x + (GetShotOffset().x);
	TargetCenter.y = FrameOutput.y + (GetShotOffset().y);

	//TargetCenter.x = FrameOutput.x + (FrameOutput.width * 0.5f);
	//TargetCenter.y = FrameOutput.y + (FrameOutput.height * 0.5f);
}

void Enemy::Render()
{

		//DoDebug();

	DrawTexturePro(ShowSpriteFlash ? SpriteFlash : GetAsset(Properties.ResourceName), 
				   FrameRec, 
				   FrameOutput, 
				   Vector2Zero(), 
				   0.f, 
				   WHITE);  // Draw part of the texture

	if (Game::Get().IsDebugMode())
	{
		DrawRectanglePro(CollisionRec, Vector2Zero(), 0.f, ALPHARED);

		if (Behaviour != nullptr)
			Behaviour->DoDebug();
	}
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
