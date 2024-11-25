#include "Particles.h"
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#include "Shots/SimpleMove.h"
#include "Shots/ChaseMove.h"
#include "Shots/LockOnMove.h"
#include "Shots/ThrottleMove.h"
#include "Shots/RotatedMove.h"
#include "Shots/PetalMove.h"
#include "Shots/DelayedMove.h"
#include "Shots/BrakeMove.h"
#include <raymath.h>
#include <math.h>

//#include <memory>

void Particles::OnInit()
{

	for (unsigned i = 0; i < TotalAmount; i++)
	{
		Bullets[i] = new Bullet();
	}

}

void Particles::OnDeinit()
{
	for (unsigned i = 0; i < TotalAmount; i++)
	{
		delete Bullets[i];
	}

	Bullets.clear();
}

void Particles::CreatePlayerShot(Vector2 position, float InitAngle, float RotationDelta)
{
	Bullet& bulletRef = *RequestBullet();

	const float speedFactor{ Graphics::Get().GetFactorArea().y };

	//bulletRef.SetType(new SimpleMove(position));
	//Vector2 center = Vector2( (float)Graphics::Get().GetHorizontalCenter(), (float)Graphics::Get().GetVerticalCenter());
	//bulletRef.SetType(new LockOnMove(position, center, 500 * speedFactor));


	if (CurrentTarget == nullptr)
	{
		bulletRef.SetType(new RotatedMove(position, InitAngle, 1000 * speedFactor, RotationDelta ));
	}
	else
	{
		bulletRef.SetType(new LockOnMove(position,
										 CurrentTarget->Position, 
										 500 * speedFactor,
										 10.f / speedFactor));
	}

	bulletRef.Active = true;
}

void Particles::Create(Vector2 position, BehaviourType bType)
{
	//----------------------------------------------
// 
	/// Bullet& bulletRef = *RequestBullet();
	static float ShotInitialAngle = 0;

	switch (bType)
	{
		case BehaviourType::ELinearShot:				/// <-
		{
			Bullet& bulletRef = *RequestBullet();
			bulletRef.SetType(new SimpleMove(position));
			bulletRef.Active = true;
		}
			break;

		case BehaviourType::ELockShot:					/// <-
		{
			Bullet& bulletRef = *RequestBullet();

			if (CurrentTarget != nullptr)
				bulletRef.SetType(new LockOnMove(position, CurrentTarget->Position, 250));
			else
				bulletRef.SetType(new LockOnMove(position, Vector2(position.x, -1000), 250));

			bulletRef.Active = true;
		}
		break;

		case BehaviourType::ESeekShot:					/// <-
		{
			Bullet& bulletRef = *RequestBullet();

			if (PlayerRef != nullptr)
				bulletRef.SetType(new ChaseMove(Vector2(200, 100), PlayerRef->Position));
			else
				bulletRef.SetType(new ChaseMove(position, Vector2(position.x, 1000)));

			bulletRef.Active = true;
		}
		break;
		case BehaviourType::EMultiShot:					 /// <-
		{
			int BurstCount = 8;

			float BurstAngleOffset = 360.f / BurstCount;

			float CurrentBurstAngle = BurstAngleOffset; 

			for (int i = 0; i < BurstCount; i++)
			{
				Bullet& bulletRef = *RequestBullet();
				bulletRef.SetType(new SimpleMove(position, CurrentBurstAngle, 300));
				CurrentBurstAngle += BurstAngleOffset;
				bulletRef.Active = true;
			}
		}
			break;
		case BehaviourType::ESpiralShot:			/// <-
		{
			float ShotAngleOffset = 10.f;
			//float ShotAngleOffset = 20.f;

			Bullet& bulletRef = *RequestBullet();

			//bulletRef.SetType(new StraightMove(position, ShotInitialAngle, 150));
			bulletRef.SetType(new SimpleMove(position, ShotInitialAngle, 100)); /// <-

			ShotInitialAngle += ShotAngleOffset;

			bulletRef.Active = true;

		}
			break;
		case BehaviourType::EMultiSpiralShot:
		{
			int BurstCount = 3;

			float BurstAngleOffset = 360.f / BurstCount;

			float ShotAngleOffset = 10.f;

			ShotInitialAngle += ShotAngleOffset;

			float CurrentBurstAngle = BurstAngleOffset + ShotInitialAngle;

			for (int i = 0; i < BurstCount; i++)
			{
				Bullet& bulletRef = *RequestBullet();
				bulletRef.SetType(new SimpleMove(position, CurrentBurstAngle, 100));
				//bulletRef.SetType(new ThrottleMove(position, CurrentBurstAngle, 350, 5, 100));
				CurrentBurstAngle += BurstAngleOffset;
				bulletRef.Active = true;
			}

		}
		break;
		case BehaviourType::ERingShot:
		{
			int BurstCount = 32;

			float BurstAngleOffset = static_cast<float>(360) / BurstCount;

			float CurrentBurstAngle = BurstAngleOffset;

			for (int i = 0; i < BurstCount; i++)
			{
				Bullet& bulletRef = *RequestBullet();
				bulletRef.SetType(new ThrottleMove(position, CurrentBurstAngle, 350, -10, 50));
				CurrentBurstAngle += BurstAngleOffset;
				bulletRef.Active = true;
			}
		}
		break;

		case BehaviourType::EStarShot:
		{
			int BurstCount = 120;

			int VertexCount = 8;

			float BurstAngleOffset = static_cast<float>(360) / BurstCount;

			float CurrentBurstAngle = BurstAngleOffset;

			for (int i = 0; i < BurstCount; i++)
			{
				Bullet& bulletRef = *RequestBullet();

				float extraSpeed = abs(sin(VertexCount * CurrentBurstAngle * PI / 360));
				bulletRef.SetType(new ThrottleMove(position, CurrentBurstAngle, 50, 1 - extraSpeed, 25)); // Classic Star

				CurrentBurstAngle += BurstAngleOffset;
				bulletRef.Active = true;
			}
		}
			break;

		case BehaviourType::EHanaShot:
		{

			int BurstCount = 6;

			float BurstAngleOffset = 360.f / BurstCount;

			float CurrentBurstAngle = BurstAngleOffset; /// Maybe define this inside Shooter entities classes?

			float AngleFactor = Graphics::Get().GetFactorArea().y > 1.0f ? 0.5f : 1.0f;

			for (int i = 0; i < BurstCount; i++)
			{
				Bullet& bulletRefRight = *RequestBullet();
				Bullet& bulletRefLeft = *RequestBullet();

				bulletRefRight.SetType(new PetalMove(position, CurrentBurstAngle, 300, AngleFactor)); // 300, 2.0f)); // smaller leaves
				bulletRefLeft.SetType(new PetalMove(position, CurrentBurstAngle, 300, -AngleFactor));

				bulletRefRight.Active = true;
				bulletRefLeft.Active = true;

				CurrentBurstAngle += BurstAngleOffset;
			}
		}
			break;
		case BehaviourType::EArchShot:
		{
			int BurstCount = 120;

			int ArchCount = 8;

			float BurstAngleOffset = 360.f / BurstCount; // 3

			float ArchAngleOffset = 360.f / ArchCount; // 45

			unsigned StepsPerArch = (unsigned)(ArchAngleOffset / BurstAngleOffset); // 15

			/// Pre-Calculate Pointed Arch 
			std::vector<Vector2> ArchValues = std::vector<Vector2>(StepsPerArch);

			for (unsigned i = 0; i < StepsPerArch; i++)
			{
				float xCoord = i * ArchAngleOffset / StepsPerArch;

				// This calculation works for half arc the other half is the same with an offset
				float offset = (xCoord < (ArchAngleOffset / 2)) ? ArchAngleOffset : 0.0f; 

				float yCoord = sqrtf( (ArchAngleOffset * ArchAngleOffset) - powf(xCoord - offset, 2));

				ArchValues[i] = Vector2(xCoord, yCoord);
			}

			// A small fix for this bastard that tends to be zero from time to time
			ArchValues[0].y = ArchValues[1].y * 0.5f; 
			
			float CurrentAngle = BurstAngleOffset;

			for (int i = 0; i < ArchCount; i++) // i < 8
			{
				for (unsigned j = 0; j < StepsPerArch; j++)// i < 15
				{
					float yHeight = ArchValues[j].y * ArchAngleOffset * 0.1f;

					Bullet& bulletRef = *RequestBullet();

					BaseBehaviour* wrappedBullet = new SimpleMove(position, CurrentAngle, 50 + yHeight);
					BaseBehaviour* delayedBullet = new DelayedMove(position, CurrentAngle, 50.0f, wrappedBullet, 1.5f);
					bulletRef.SetType(new BrakedMove(position, CurrentAngle, 200, delayedBullet, 10.0f)); // + complex

					bulletRef.Active = true;

					CurrentAngle += BurstAngleOffset;
				}
			}
		}
			break;
		case BehaviourType::EEnemyShot:
			break;
		case BehaviourType::EPlayerShot:
			break;
		case BehaviourType::EExplosion:
			break;
		default:
			break;
	}


	/// bulletRef.Active = true;
}

Bullet* Particles::RequestBullet()
{
	if (Bullets.empty())
		return nullptr;

	CurrentBulletIndex %= TotalAmount;

	Bullet* BulletRef = Bullets[CurrentBulletIndex];

	BulletRef->Active = false;

	CurrentBulletIndex++;

	return BulletRef;
}


void Particles::Disable(Bullet* bullet)
{
	bullet->Active = false;
}

void Particles::OnUpdate()
{

	for (Bullet* bullet : Bullets)
	{
		if (bullet->Active)
		{
			bullet->Update();
		}
	}
}

void Particles::OnRender()
{
	for (Bullet* bullet : Bullets)
	{
		if (bullet->Active)
		{
			bullet->Render();
		}
	}
}

void Particles::OnPause()
{
}

void Particles::OnResume()
{
}

