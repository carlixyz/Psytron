#pragma once

#include "../../Utility/Singleton.h"
#include "../../Graphics/Graphics.h"
//#include <iostream>
#include <vector>

struct Entity;
struct Enemy;
struct Bullet;
struct BaseBehaviour;

class Particles : public Singleton<Particles>
{

	friend class Singleton<Particles>;

	// --------------------------------- GENERAL

	const unsigned TotalAmount = 10000;

	std::vector<Bullet*> Bullets		{ TotalAmount };

	unsigned CurrentBulletIndex = 0;

	Bullet* RequestBullet();

	const float speedFactor				{ Graphics::Get().GetFactorArea().y };

public:
	void OnInit();
	void OnDeinit();


	void OnUpdate(std::vector<Enemy*>& enemies);
	void OnRender();

	void OnPause();
	void OnResume();

	void Create(Vector2 Position, enum class BehaviourType bType/*, Entity* target = nullptr*/); /// Abstract Factory trough A Function :

	void CreatePlayerShot(Vector2 startPosition);

	Entity* CurrentTarget = nullptr;

	Entity* PlayerRef = nullptr;

	void Disable(Bullet* bullet);

};

