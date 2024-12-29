#pragma once

#include "../../Utility/Singleton.h"
#include "Enemy.h"
#include <vector>
#include <optional>
#include <list>
#include <iostream>
#include "raylib-cpp.hpp"



class Swarm // It's like a group or Wave of enemies
{

public:
    //Swarm(std::vector<Enemy*>& enemies) : Enemies(std::move(enemies)) {}
    Swarm(std::vector<Enemy*>& enemies) : Enemies(enemies) {}

    ~Swarm() { DeInit(); }

    void SpawnEnemies() 
    {
        for (auto& enemy : Enemies)
        {
            if (enemy->Properties.Life > 0)
                enemy->Active = true;
        }
    }

    void UpdateEnemies()
    {
        for (auto& enemy : Enemies)
        {
            if (enemy->Active)
                enemy->Update();
        }
    }

    bool AllEnemiesDestroyed() const 
    {
        for (const auto& enemy : Enemies)
        {
            if (enemy->Active)
            {
                return false; // At least one enemy is still alive
            }
        }
        return true; // All enemies are destroyed (or disabled)
    }
protected:
    friend class EnemyHandler;
    void DeInit()
    {
        for (Enemy* enemy : Enemies)
        {
            delete enemy;
        }

        Enemies.clear();
    }

    const std::vector<Enemy*>& GetEnemies() 
    { 
        return Enemies; 
    }

    //int BlastNumber;
    //std::vector<std::shared_ptr<Enemy>> Enemies;
    std::vector<Enemy*> Enemies;
};



class EnemyHandler //: public Singleton<EnemyManager>
{
	//friend class Singleton<EnemyManager>;

public:
    void Init();

    void CreateFinalWave();

    void CreateFourthWave();

    void Update();

    void Render();

    void Deinit();

    const std::optional<std::vector<Enemy*>> GetCurrentWave();

    unsigned GetCurrentWaveIndex() { return CurrentWaveIndex; }

    unsigned GetSpeedRequest() { return SpeedRequest; }
private:

    void CreateIntroWave();

    void CreateFirstWave();

    void CreateSecondWave();

    void CreateThirdWave();

    void CreateFifthWave();

    void CreateSixthWave();

    void AddWave(std::vector<Enemy*>& enemies);

    void SpawnCurrentWave();

    std::list<Swarm*> Swarms;

    bool checkWaveCompletion();

    bool SpeedRequest = false;

    unsigned CurrentWaveIndex = 0;
};

