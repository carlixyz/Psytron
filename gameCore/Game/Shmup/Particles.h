#pragma once

#include "../../Utility/Singleton.h"
#include "../../Graphics/Graphics.h"
//#include <iostream>
#include <vector>

struct Entity;
struct Bullet;
struct BaseBehaviour;

class Particles : public Singleton<Particles>
{

	friend class Singleton<Particles>;

	//struct Player* playerRef = nullptr;

	// --------------------------------- GENERAL

	const unsigned TotalAmount = 10000;

	std::vector<Bullet*> Bullets{ TotalAmount };

	unsigned CurrentBulletIndex = 0;

	Bullet* RequestBullet();

public:
	void OnInit();
	void OnDeinit();


	void OnUpdate();
	void OnRender();

	void OnPause();
	void OnResume();

	void Create(Vector2 Position, enum class BehaviourType bType/*, Entity* target = nullptr*/); /// Abstract Factory trough A Function :

	void CreatePlayerShot(Vector2 position, float InitAngle, float RotationDelta);

	Entity* CurrentTarget = nullptr;

	Entity* PlayerRef = nullptr;

	void Disable(Bullet* bullet);

};

