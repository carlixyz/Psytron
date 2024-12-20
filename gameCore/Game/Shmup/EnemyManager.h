#pragma once

#include "../../Utility/Singleton.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include "raylib-cpp.hpp"



class Wave {
public:
    Wave(int waveNumber, int enemyCount) : waveNumber(WaveNumber) {
        for (int i = 0; i < enemyCount; ++i)
        {
            enemies.push_back(std::make_shared<Enemy>(i + WaveNumber * 100)); // Unique ID per wave
        }
    }

    void spawnEnemies() {
        for (auto& enemy : Enemies)
        {
            //enemy->OnIni();
        }
    }

    void updateEnemies() {
        for (auto& enemy : Enemies)
        {
            enemy->Update();
        }
    }

    bool allEnemiesDestroyed() const {
        for (const auto& enemy : Enemies)
        {
            //if (!enemy->isEnemyDestroyed())
            {
                return false; // At least one enemy is still alive
            }
        }
        return true; // All enemies are destroyed
    }

private:
    int WaveNumber;
    std::vector<std::shared_ptr<Enemy>> Enemies;
};


class EnemyManager : public Singleton<EnemyManager>
{
	friend class Singleton<EnemyManager>;


public:
    void addWave(int enemyCount) {
        waves.push_back(std::make_shared<Wave>(currentWave++, enemyCount));
    }

    void spawnCurrentWave() {
        if (!waves.empty())
        {
            waves.back()->spawnEnemies();
        }
    }

    void updateCurrentWave() {
        if (!waves.empty())
        {
            waves.back()->updateEnemies();
            checkWaveCompletion(); // Check if the current wave is complete
        }
    }

    void destroyEnemy(int enemyId) {
        // Simulate destroying an enemy with a specific ID
        for (auto& enemy : waves.back()->getEnemies())
        {
            if (enemy->getId() == enemyId)
            {
                enemy->destroy();
                break;
            }
        }
    }

private:
    int currentWave = 0;
    std::vector<std::shared_ptr<Wave>> waves;

    void checkWaveCompletion() {
        if (waves.back()->allEnemiesDestroyed())
        {
            std::cout << "All enemies in wave " << currentWave - 1 << " destroyed. Moving to next wave." << std::endl;
            waves.pop_back(); // Remove the completed wave
            if (!waves.empty())
            {
                spawnCurrentWave(); // Spawn the next wave
            }
        }
    }


};

