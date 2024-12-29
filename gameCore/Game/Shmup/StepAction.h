#pragma once

#include <raylib-cpp.hpp>
#include <raymath.h>
#include "Bullet.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Particles.h"
#include <memory>


/// <summary>
///  BaseBehavior subset to customize Enemy actions in circular sequence (check Enemy.h)
/// </summary>
struct IStepAction : public BaseBehaviour
{
	virtual void DoInit()		override { if (Owner != nullptr) EnemyOwner = (Enemy*)Owner; }
	//virtual void DoUpdate()	override { ; }
	//virtual void DoRender()	override { ; }
	//virtual void DoDebug()	override { ; }
	virtual IStepAction* Clone() const = 0;

	bool Done				= false;
	Enemy* EnemyOwner		= nullptr;

	void ProcessNextStep()
	{ 
		Done = true; 
		if (EnemyOwner)
			EnemyOwner->ProcessStep();
	}
};


struct StepShoot : public IStepAction
{
	/// <summary>
	/// Shoots something for a number of times with a fire rate.
	/// </summary>
	/// <param name="shotType"></param>
	/// <param name="times"></param>
	StepShoot(BehaviourType shotType = BehaviourType::ENone, int times = 1, float fireRate = 0.5f) :
		ShotType(shotType), TimesCount(times), Times(times), FireRateLapse(fireRate) {}

	virtual IStepAction* Clone() const { return new StepShoot(*this); }

	BehaviourType ShotType;
	int TimesCount;					// Total blasts amount
	int Times;						// Starting blast count
	float FireRateLapse = 0.25f;
	float FireRate = 0.f;

	inline virtual void DoUpdate() override
	{
		FireRate += GetFrameTime();
		if (FireRate > FireRateLapse)
		{
			Particles::Get().Create(Vector2Add(EnemyOwner->Position, EnemyOwner->GetShotOffset()), ShotType);
			Times--;

			if (Times <= 0)
			{
				Times = TimesCount;
				ProcessNextStep();
				//return;
			}

			FireRate = 0.f;
		}
	}

	virtual void DoDebug() override
	{
		DrawCircle((int)Vector2Add(EnemyOwner->Position, EnemyOwner->GetShotOffset()).x,
				   (int)Vector2Add(EnemyOwner->Position, EnemyOwner->GetShotOffset()).y, 3, GREEN);
		//DrawCircle((int)EnemyOwner->Position.x, (int)EnemyOwner->Position.y, 3, reach ? GREEN : WHITE);
	}
};

struct StepMoveTowards : public IStepAction
{
	/// <summary>
	/// Moves towards an absolute Vector2 position in the screen
	/// </summary>
	/// <param name="target"></param>
	/// <param name="speed"></param>
	StepMoveTowards(Vector2 target = Graphics::Get().GetScreenCenter(),	float speed = 300.0f) :
		TargetPosition(target),
		Speed(speed) {}

	virtual IStepAction* Clone() const { return new StepMoveTowards(*this); }

	float Speed;
	Vector2 TargetPosition;

	virtual void DoUpdate() override 
	 {
		 if (EnemyOwner == nullptr)
			 return;

		 if (GoToTarget())
			 ProcessNextStep();
	 }

	bool GoToTarget()
	 {
		 bool TargetReached = false;

		 // Calculate the distance vector 
		 // (diff between the tracker position tracker and target position to be tracked)
		 Vector2 Distance = Vector2Subtract(TargetPosition, EnemyOwner->Position);

		 float TargetSpeed = Clamp((Vector2Length(Distance) / 128) * Speed, 16, Speed);

		 // Calculate the distance the pursuer will move taking into account its max speed
		 float displacement = TargetSpeed * GetFrameTime();

		 // If the distance the pursuer is going to move is greater than or equal 
		 // to the distance to its target, it moves only up to the target point
		 // (so as not to go over). Otherwise, it moves the established distance
		 // (to gradually get closer to the target)
		 if (displacement >= Vector2Length(Distance))
		 {
			 //EnemyOwner->Position = TargetPosition;
			 TargetReached = true;
		 }
		 else
		 {
			 // The new position of the pursuer will be equal to its initial position
			 // plus the displacement it must make in the direction indicated by distance vector 
			 // (to move in the direction of its target)
			 Vector2 offset = Vector2Scale(Vector2Normalize(Distance), displacement);
			 Vector2 deltaPosition = Vector2Add(EnemyOwner->Position, offset);
			 EnemyOwner->Position = deltaPosition;
		 }

		 EnemyOwner->CollisionRec.x = EnemyOwner->Position.x - EnemyOwner->CollisionRec.width * 0.5f;
		 EnemyOwner->CollisionRec.y = EnemyOwner->Position.y - EnemyOwner->CollisionRec.height * 0.5f;

		 return TargetReached;
	 }

	virtual void DoDebug() override
	{ 
		DrawCircle((int)TargetPosition.x, (int)TargetPosition.y, 3, RED);
		DrawCircle((int)EnemyOwner->Position.x, (int)EnemyOwner->Position.y, 3, GREEN);
	}
};

struct StepMoveRelative : public StepMoveTowards
{
	/// <summary>
	/// Moves slightly on towards a target relative to previous position + offset
	/// </summary>
	/// <param name="speed"></param>
	StepMoveRelative(Vector2 offset = Vector2(0, 200), float speed = 200.0f) :
		StepMoveTowards(Vector2Zero(), speed), Offset(offset) {}

	virtual IStepAction* Clone() const { return new StepMoveRelative(*this); }

	bool PositionFetched = false;
	Vector2 Offset = Vector2Zero();

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (!PositionFetched)
		{
			PositionFetched = true;
			TargetPosition = Vector2Add(EnemyOwner->Position, Offset);
		}

		if (GoToTarget())
		{
			PositionFetched = false;
			ProcessNextStep();
		}
	}
};

struct StepMoveAbovePlayer : public StepMoveTowards
{
	/// <summary>
	/// Moves slightly above the player position (with an offset)
	/// </summary>
	/// <param name="speed"></param>
	StepMoveAbovePlayer(float speed = 200.0f, Vector2 offset = Vector2(-32, -200)) :
		StepMoveTowards(Vector2Zero(), speed), Offset(offset) {}

	virtual IStepAction* Clone() const { return new StepMoveAbovePlayer(*this); }

	bool PositionFetched = false;
	Vector2 Offset = Vector2Zero();

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (!PositionFetched)
		{
			PositionFetched = true;

			Vector2 PlayerPosition = Particles::Get().PlayerRef != nullptr ?
				Vector2Add(Particles::Get().PlayerRef->Position, Offset) :
				Vector2Add(EnemyOwner->Position, Offset);

			TargetPosition = PlayerPosition;
		}

		if (GoToTarget())
		{
			PositionFetched = false;
			ProcessNextStep();
		}
	}
};




/*
struct StepMoveAndShoot : public StepMoveTowards
{
	/// <summary>
	/// new StepMoveAndShoot(NextPosition, 100, BehaviourType::ESimpleMove, 1)
	/// </summary>
	/// <param name="target"></param>
	/// <param name="speed"></param>
	/// <param name="shotType"></param>
	/// <param name="times"></param>
	StepMoveAndShoot( Vector2 target, float speed = 100.0f, BehaviourType shotType = BehaviourType::ENone, int times = 1) :
		StepMoveTowards(target, speed),
		ShotType(shotType),
		Times(times) {}

	//float Speed;
	//Vector2 TargetPosition;
	BehaviourType ShotType;
	int Times;

	//virtual void DoUpdate();
};
*/


struct StepWait : public IStepAction
{
	/// <summary>
	/// Wait in Idle for a specific amount of time
	/// </summary>
	/// <param name="wait"></param>
	StepWait(float wait = 1.0f, bool repeat = true) : WaitTime(wait), Repeat(repeat){}

	virtual IStepAction* Clone() const { return new StepWait(*this); }

	float WaitTime = 1.0f;
	float LapseWait = 0.f;
	bool Repeat = true;
	bool OneShot = false;

	inline virtual void DoUpdate() override
	{
		LapseWait += GetFrameTime();
		if (OneShot || LapseWait > WaitTime)
		{
			if (!Repeat && !OneShot)
			{
				OneShot = true;
			}

			LapseWait = 0.f;
			ProcessNextStep();
		}
	}
};

struct StepExit : public StepMoveAbovePlayer
{
	/// <summary>
	/// The Enemy exits the screen area and disables itself
	/// </summary>
	/// <param name="speed"></param>
	StepExit(float speed = 200.0f) : StepMoveAbovePlayer(speed) {}

	virtual IStepAction* Clone() const { return new StepExit(*this); }

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (!PositionFetched)
		{
			PositionFetched = true;
			TargetPosition = Vector2(EnemyOwner->Position.x, Graphics::Get().GetWindowArea().height + 256);
		}

		if (GoToTarget())
		{
			EnemyOwner->Active = false;
			EnemyOwner->Properties.Life = 0.f;
			Done = false;
			//ProcessNextStep();
		}
	}
};


struct StepSlideTowards : public IStepAction
{
	/// <summary>
	/// unlike MoveTowards, this Slides Indirectly towards an absolute target 
	/// </summary>
	/// <param name="target"></param>
	/// <param name="speed"></param>
	StepSlideTowards(Vector2 target = Graphics::Get().GetScreenCenter(),
					 float speed = 300.0f) :
		TargetPosition(target),
		Speed(speed) {}

	virtual IStepAction* Clone() const { return new StepSlideTowards(*this); }

	Vector2 TargetPosition;
	Vector2 Direction		= Vector2One();

	float Speed				= 200;
	float AngSpeed			= 15;
	float AngAcumulation	= 0.f;

	float ReachDistance		= 10;
	float BrakeDistance		= 128;	// 64
	float MinSpeed			= 16;	// 8

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (GoToTarget())
		{
			ProcessNextStep();
		}
	}

	bool GoToTarget()
	{
		bool TargetReached = false;
		const float timeLapse = GetFrameTime();

		Vector2 Distance = Vector2Subtract(TargetPosition, EnemyOwner->Position);

		// Calculate the forward vector : The forward vector is obtained by subtracting the object’s position from the target position, resulting in a vector that points from the object toward the target.
		Vector2 Forward = Vector2Subtract(Vector2Add(EnemyOwner->Position, Direction), EnemyOwner->Position);

		// Normalize the forward vector : Normalizing the forward vector ensures that it has a length of 1, preserving the object’s scale and preventing undesired distortions.
		Vector2 NormalizedDist = Vector2Normalize(Distance);
		Forward = Vector2Normalize(Direction);

		///----------------------------------------------------------------- ///
		AngAcumulation = Lerp(AngAcumulation, AngSpeed, timeLapse);

		Direction = Vector2Lerp(Forward, NormalizedDist, AngAcumulation * timeLapse);

		float DistanceLength = Vector2Length(Distance);

		if (DistanceLength < ReachDistance)
		{
			TargetReached = true;
			Direction = Vector2One();
		}

		if (EnemyOwner)
		{
			float TargetSpeed = Clamp((DistanceLength / BrakeDistance) * Speed, MinSpeed, Speed);

			EnemyOwner->Position.x += Direction.x * TargetSpeed * timeLapse;
			EnemyOwner->Position.y += Direction.y * TargetSpeed * timeLapse;

			EnemyOwner->CollisionRec.x = EnemyOwner->Position.x - EnemyOwner->CollisionRec.width * 0.5f;
			EnemyOwner->CollisionRec.y = EnemyOwner->Position.y - EnemyOwner->CollisionRec.height * 0.5f;
		}

		return TargetReached;
	}

	virtual void DoDebug() override
	{
		DrawCircle((int)TargetPosition.x, (int)TargetPosition.y, 3, RED);
		DrawCircle((int)EnemyOwner->Position.x, (int)EnemyOwner->Position.y, 3, GREEN);
	}
};


struct StepEnterScene : public IStepAction
{
	/// <summary>
	/// Moves towards an absolute Vector2 position in the screen
	/// </summary>
	/// <param name="target"></param>
	/// <param name="speed"></param>
	StepEnterScene(Vector2 target = Graphics::Get().GetScreenCenter(), float speed = 300.0f) :
		TargetPosition(target),
		Speed(speed) {}

	virtual IStepAction* Clone() const { return new StepEnterScene(*this); }

	float Speed;
	Vector2 TargetPosition;
	bool OneShot = false;

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (OneShot || GoToTarget())
		{
			ProcessNextStep();
		}
	}

	bool GoToTarget()
	{
		bool TargetReached = OneShot;

		// Calculate the distance vector 
		// (diff between the tracker position tracker and target position to be tracked)
		Vector2 Distance = Vector2Subtract(TargetPosition, EnemyOwner->Position);

		float TargetSpeed = Clamp((Vector2Length(Distance) / 128) * Speed, 16, Speed);

		// Calculate the distance the pursuer will move taking into account its max speed
		float displacement = TargetSpeed * GetFrameTime();

		// If the distance the pursuer is going to move is greater than or equal 
		// to the distance to its target, it moves only up to the target point
		// (so as not to go over). Otherwise, it moves the established distance
		// (to gradually get closer to the target)
		if (displacement >= Vector2Length(Distance))
		{
			//EnemyOwner->Position = TargetPosition;
			TargetReached = true;
			OneShot = true;
		}
		else
		{
			// The new position of the pursuer will be equal to its initial position
			// plus the displacement it must make in the direction indicated by distance vector 
			// (to move in the direction of its target)
			Vector2 offset = Vector2Scale(Vector2Normalize(Distance), displacement);
			Vector2 deltaPosition = Vector2Add(EnemyOwner->Position, offset);
			EnemyOwner->Position = deltaPosition;
		}

		EnemyOwner->CollisionRec.x = EnemyOwner->Position.x - EnemyOwner->CollisionRec.width * 0.5f;
		EnemyOwner->CollisionRec.y = EnemyOwner->Position.y - EnemyOwner->CollisionRec.height * 0.5f;

		return TargetReached;
	}

	virtual void DoDebug() override
	{
		DrawCircle((int)TargetPosition.x, (int)TargetPosition.y, 3, RED);
		DrawCircle((int)EnemyOwner->Position.x, (int)EnemyOwner->Position.y, 3, GREEN);
	}
};


/*
struct StepSlideRelative : public StepSlideTowards
{
	/// <summary>
	/// unlike MoveTowards, this Slides Indirectly towards a relative target 
	/// </summary>
	/// <param name="target"></param>
	/// <param name="speed"></param>
	StepSlideRelative(Vector2 offset = Vector2(0, 200), float speed = 300.0f) :
		StepSlideTowards(Vector2Zero(), speed), Offset(offset){}

	virtual IStepAction* Clone() const { return new StepSlideRelative(*this); }

	Vector2 Offset	= Vector2Zero();
	bool PositionFetched	= false;

	virtual void DoUpdate() override
	{
		if (EnemyOwner == nullptr)
			return;

		if (!PositionFetched)
		{
			PositionFetched = true;
			TargetPosition = Vector2Add(EnemyOwner->Position, Offset);
		}

		if (GoToTarget())
		{
			ProcessNextStep();
			PositionFetched = false;
		}
	}

	bool GoToTarget()
	{
		bool TargetReached = false;
		const float timeLapse = GetFrameTime();

		Vector2 Distance = Vector2Subtract(TargetPosition, EnemyOwner->Position);

		// Calculate the forward vector : The forward vector is obtained by subtracting the object’s position from the target position, resulting in a vector that points from the object toward the target.
		Vector2 Forward = Vector2Subtract(Vector2Add(EnemyOwner->Position, Direction), EnemyOwner->Position);

		// Normalize the forward vector : Normalizing the forward vector ensures that it has a length of 1, preserving the object’s scale and preventing undesired distortions.
		Vector2 NormalizedDist = Vector2Normalize(Distance);
		Forward = Vector2Normalize(Direction);

		///----------------------------------------------------------------- ///
		AngAcumulation = Lerp(AngAcumulation, AngSpeed, timeLapse);

		//Direction = Vector2Lerp(Forward, Distance, AngSpeed * timeLapse);
		Direction = Vector2Lerp(Forward, NormalizedDist, AngAcumulation * timeLapse);

		float DistanceLength = Vector2Length(Distance);

		if (DistanceLength < ReachDistance)
		{
			TargetReached = true;
			Direction = Vector2Up();
		}

		if (EnemyOwner)
		{
			float TargetSpeed = Clamp((DistanceLength / BrakeDistance) * Speed, MinSpeed, Speed);

			EnemyOwner->Position.x += Direction.x * TargetSpeed * timeLapse;
			EnemyOwner->Position.y += Direction.y * TargetSpeed * timeLapse;

			EnemyOwner->CollisionRec.x = EnemyOwner->Position.x - EnemyOwner->CollisionRec.width * 0.5f;
			EnemyOwner->CollisionRec.y = EnemyOwner->Position.y - EnemyOwner->CollisionRec.height * 0.5f;
		}

		return TargetReached;
	}

	virtual void DoDebug() override
	{
		DrawCircle((int)TargetPosition.x, (int)TargetPosition.y, 3, RED);
		DrawCircle((int)EnemyOwner->Position.x, (int)EnemyOwner->Position.y, 3, GREEN);
	}
};
*/



