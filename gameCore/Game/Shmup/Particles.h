#pragma once

#include "../../Utility/Singleton.h"
#include "../../Graphics/Graphics.h"
//#include <iostream>
#include <vector>
#include <optional>

struct Entity;
struct Player;
struct Enemy;
struct Bullet;
struct BaseBehaviour;

class Particles : public Singleton<Particles>
{

	friend class Singleton<Particles>;

	// --------------------------------- GENERAL

	const unsigned TotalAmount = 8000;

	std::vector<Bullet*> Bullets		{ TotalAmount };

	unsigned CurrentBulletIndex = 0;

	Bullet* RequestBullet();

	// --------------------------------- PLAYER

	const unsigned TotalPlayerAmount = 2000;

	std::vector<Bullet*> PlayerBullets{ TotalPlayerAmount };

	unsigned CurrentPlayerBulletIndex = 0;

	Bullet* RequestPlayerBullet();

	const float speedFactor				{ Graphics::Get().GetFactorArea().y };

public:
	void OnInit();
	void OnDeinit();

	void OnUpdate(const std::optional<std::vector<Enemy*>>& enemies);
	void OnRender();

	void OnPause();
	void OnResume();

	void Create(Vector2 Position, enum class BehaviourType bType/*, Entity* target = nullptr*/); /// Abstract Factory trough A Function :

	void CreatePlayerShot(Vector2 startPosition);

	Enemy* CurrentTarget = nullptr;

	Player* PlayerRef = nullptr;

	//void Disable(Bullet* bullet);

};

