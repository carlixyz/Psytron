#include "Boss.h"

#include <raylib-cpp.hpp>
#include "Bullet.h"
#include "Player.h"
#include "Particles.h"
#include "StepAction.h"
#include "reasings.h"
#include "Entity.h"
#include "../../Audio/Audio.h"


Boss::Boss(const Enemy& other)
{
	Properties = other.Properties;
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


void Boss::Init()
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

	if (!Steps.empty())
		SetCurrentAction(Steps[CurrentStepIndex]);

	IsShieldEnabled = true;

	Audio::Get().PlayMusic("Data/Sound/stg_st003_88pro.mp3", false);
}

void Boss::Update()
{
	if (!IsShieldEnabled && DoDamageFlash)
		UpdateDamage();

	if (Behaviour != nullptr)
		Behaviour->DoUpdate();


	UpdateRectangles();
	
	StateChange -= GetFrameTime();
	if (StateChange < 0)
	{
		StateChange = StateDelay;
		DoingTransition = true;
	}

	UpdateShieldState();
}

void Boss::UpdateShieldState()
{
	FramesCounter++;

	if (FramesCounter >= (60 / Speed))
	{
		if (DoingTransition)
		{
			if (IsShieldEnabled)
			{
				FramesCounter = 0;
				CurrentCol++;

				if (CurrentCol > 3)
				{
					CurrentCol = 3;
					IsShieldEnabled = false;
					DoingTransition = false;
				}
			}
			else
			{
				FramesCounter = 0;
				CurrentCol--;

				if (CurrentCol < 0 && DoingTransition)
				{
					CurrentCol = 0;
					IsShieldEnabled = true;
					DoingTransition = false;
				}
			}
		}
	}

	FrameRec.x = CurrentCol * SpriteSize.x;

}


bool Boss::ApplyDamage()
{
	if (!IsShieldEnabled)
	{
		DoDamageFlash = true;
		Properties.Life -= Properties.Damage;
	}

	if (Properties.Life <= 750)
		StateDelay = 15.f;

	if (Properties.Life <= 500)
		StateDelay = 20.f;

	if (Properties.Life <= 250)
		StateDelay = 30.f;


	if (Properties.Life <= 0)
	{
		const Vector2 explotion(Position.x + FrameOutput.width * 0.5f, Position.y + FrameOutput.height * 0.5f);
		Particles::Get().Create(explotion, BehaviourType::EExplotion);
		return false;	// Active = false;
	}

	return true;
}

void Boss::Render()
{

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
