#include "EnemyHandler.h"

#include "StepAction.h"
#include "../../Utility/Utils.h"
#include "../../Graphics/Graphics.h"
#include "raylib-cpp.hpp"
#include <functional>
#include "Boss.h"


const float screenFactor  = Graphics::Get().GetFactorArea().x;
const float Size_512  = 256 * screenFactor;
const float Size_256  = 128 * screenFactor;
const float Size_128  = 64 * screenFactor;
const float Size_64   = 32 * screenFactor;
const float Size_32   = 16 * screenFactor;
const float Size_320  = 160 * screenFactor;
const float Size_384  = 192 * screenFactor;
const float Size_448  = 224 * screenFactor;
const float Size_200  = 100 * screenFactor;

#define OUT_TOP_CENTER	Vector2(Graphics::Get().GetScreenCenter().x-(Size_128), -Size_512)
#define OUT_TOP_LEFT	Vector2(Graphics::Get().GetScreenCenter().x-Size_448, -Size_512)
#define OUT_TOP_RIGHT	Vector2(Graphics::Get().GetScreenCenter().x+Size_200, -Size_512)
#define OUT_TOP_LEFT_CORNER	    (Vector2(-Size_128, -Size_128))
#define OUT_TOP_RIGHT_CORNER    (Vector2(Graphics::Get().GetWindowArea().width, -Size_128))
#define OUT_TOP_LEFT_ROAD	Vector2(Graphics::Get().GetScreenCenter().x-Size_384, -Size_512)
#define OUT_TOP_RIGHT_ROAD	Vector2(Graphics::Get().GetScreenCenter().x+Size_320, -Size_512)

#define OUT_LOW_CENTER	Vector2(Graphics::Get().GetScreenCenter().x-Size_128, Graphics::Get().GetWindowArea().height)
#define OUT_LOW_LEFT	Vector2(Graphics::Get().GetScreenCenter().x-Size_320, Graphics::Get().GetWindowArea().height)
#define OUT_LOW_RIGHT	Vector2(Graphics::Get().GetScreenCenter().x+Size_256, Graphics::Get().GetWindowArea().height)

#define POS_CENTER		Vector2Add(Graphics::Get().GetScreenCenter(), Vector2(-Size_64, -Size_64))
#define POS_RIGHT		Vector2Add(POS_CENTER, Vector2( Size_320, 0		    ))
#define POS_LEFT		Vector2Add(POS_CENTER, Vector2( -(Size_320+Size_64) , 0	))

#define POS_TOP_CENTER	Vector2Add(Graphics::Get().GetScreenCenter(), Vector2(0, -Size_256))
#define POS_TOP_RIGHT	Vector2Add(POS_CENTER, Vector2( Size_256 + Size_64, -Size_256	))
#define POS_TOP_LEFT	Vector2Add(POS_CENTER, Vector2( -(Size_256 + Size_64), -Size_256 ))

#define POS_LOW_CENTER	Vector2Add(Graphics::Get().GetScreenCenter(), Vector2(-Size_128, Size_256))
#define POS_LOW_RIGHT	Vector2Add(POS_CENTER, Vector2( Size_256 + Size_64, Size_256	))
#define POS_LOW_LEFT	Vector2Add(POS_CENTER, Vector2( -(Size_256 + Size_64),Size_256   ))


#define ROAMER_SPR  (Vector2(0,1))
#define BUNGIE_SPR  (Vector2One())
#define RAZOR_SPR   (Vector2(2,1))
#define VOLBO_SPR   (Vector2(3,1))
#define SEEKER_SPR  (Vector2(4,1))
#define SWEPER_SPR  (Vector2(5,1))
#define CUSTOM_SPR  (Vector2Zero())
#define BOSS_SPR    (Vector2(0,3))

#define BUNGIE(x)  { x, BUNGIE_SPR, 120, 4, Vector2(.35f,.35f)  }
#define ROAMER(x)  { x, ROAMER_SPR, 150, 5, Vector2(.45f,.45f)  }
#define SWEPER(x)  { x, SWEPER_SPR, 60, 5, Vector2(.45f,.45f)  }
#define RAZOR(x)   { x, RAZOR_SPR, 300, 2, Vector2(.3f, .3f)    }
#define VOLBO(x)   { x, VOLBO_SPR, 100, 5, Vector2(.35f, .35f)  }
#define SEEKER(x)  { x, SEEKER_SPR, 400, 4, Vector2(.35f, .35f) }
#define BOSS(x)    { x, BOSS_SPR, 500, 2, Vector2(.45f, .45f), 0, 4 }

#define CAR_A(x)  { x, CUSTOM_SPR, 100, 10, Vector2(0.75f, 0.6f), 0, 1, "Car1" }
#define CAR_B(x)  { x, CUSTOM_SPR, 100, 10, Vector2(0.75f, 0.7f), 0, 1, "Car2" }
#define CAR_C(x)  { x, CUSTOM_SPR, 100, 10, Vector2(0.75f, 0.7f), 0, 1, "Car4" }
#define CAR_D(x)  { x, CUSTOM_SPR, 100, 5, Vector2(0.55f, 0.65f), 0, 1, "Car5" }

#define BLOCK_A(x)  { x, CUSTOM_SPR, 100, 10, Vector2(0.9f, 0.65f), 0, 1, "BlockA" }     // Barricade
#define BLOCK_B(x)  { x, CUSTOM_SPR, 100, 12, Vector2(0.75f, 0.6f), 0, 1, "BlockB" }     // Sign
#define BLOCK_C(x)  { x, CUSTOM_SPR, 100, 15, Vector2( 0.8f, 0.8f), 0, 1, "BlockC" }    // Cone
#define BLOCK_D(x)  { x, CUSTOM_SPR, 100, 15, Vector2( 0.6f, 0.6f), 0, 1, "BlockD" }    // Wheel
#define BLOCK_E(x)  { x, CUSTOM_SPR, 100, 12, Vector2(0.85f, 0.75f), 0, 1,"BlockE" }     // Barrier

void EnemyHandler::Init()
{
    /// In a real case on a daily basis I should include below here some data driven loading logic 
    /// However I'm just damn tired of this project which I spent time waaay far beyond than meaningful
    ///
    /// 
    CurrentWaveIndex = 0;

    CreateIntroWave();

    CreateFirstWave();

    CreateSecondWave();

    CreateThirdWave();

    CreateFourthWave();

    CreateFifthWave();

    CreateSixthWave();

    CreateFinalWave();

    SpawnCurrentWave();
}

void EnemyHandler::CreateIntroWave()
{
    std::vector<IStepAction*> BungieActions{
        new StepWait(10.0f, false),
        new StepMoveTowards(POS_TOP_CENTER),
        new StepShoot(BehaviourType::EAimShot, 3, 0.5f),
        new StepWait(0.2f),
        new StepSlideTowards(Vector2Add(POS_RIGHT, Vector2(0, -50 * screenFactor))),
        new StepWait(0.2f),
        new StepShoot(BehaviourType::EAimShot, 3, 0.25f),
        new StepWait(0.2f),
        new StepMoveAbovePlayer(500.f, Vector2(-Size_32, -Size_128)),
        new StepShoot(BehaviourType::EAimShot, 3, 0.25f),
        new StepWait(1.0f)
    };

    EnemyProperties BungieStats = 
        { OUT_TOP_RIGHT_CORNER, BUNGIE_SPR, 150, 4, Vector2(.3f, .3f) };

    std::vector<Enemy*> IntroSwarm = {
        new Enemy(BungieStats, BungieActions)
    };

    AddWave(IntroSwarm);
}

void EnemyHandler::CreateFirstWave()
{
    std::vector<IStepAction*> BungieActions{
        new StepWait(1.0f),
        new StepMoveTowards(POS_CENTER),
        new StepShoot(BehaviourType::EMultiShot, 3, 1.0f),
        new StepWait(0.25f),
        new StepSlideTowards(Vector2Add(POS_RIGHT, Vector2(0, -50 * screenFactor))),
        new StepWait(0.5f),
        new StepShoot(BehaviourType::EAimShot, 5, 0.5f),
        new StepWait(0.25f),
        new StepMoveAbovePlayer(350.f, Vector2(-Size_32, -150 * screenFactor))
    };

    EnemyProperties BungieStats = BUNGIE(OUT_TOP_CENTER);
    auto bungieA = new Enemy(BungieStats, BungieActions);
    BungieStats.InitPosition = OUT_TOP_RIGHT;
    BungieStats.StartIndex = 4;
    auto bungieB = new Enemy(BungieStats, BungieActions);

    std::vector<Enemy*> FirstSwarm = {
        bungieA,
        bungieB
    };

    AddWave(FirstSwarm);
}

void EnemyHandler::CreateSecondWave()
{
    std::vector<IStepAction*> StepActionsLeft = {
        new StepWait(5.0f, false),  // 5.0
        new StepMoveTowards(Vector2(Graphics::Get().GetScreenCenter().x - 170 * screenFactor, Size_256)),
        new StepShoot(BehaviourType::EAimShot, 3, 0.5f),
        new StepMoveTowards(OUT_LOW_LEFT),
        new StepMoveRelative(Vector2(-Size_512, 0)),
        new StepWait(1.0f),
        new StepMoveTowards(Vector2(-Size_128, -Size_128))
    };

    auto RoamerLeftA = new Enemy({ ROAMER(OUT_TOP_LEFT_CORNER), StepActionsLeft });

    StepWait* StepWaitLeft = (StepWait*)StepActionsLeft[0];
    StepWaitLeft->WaitTime = 12.0f;  // 12.0
    auto RoamerLeftB = new Enemy({ ROAMER(OUT_TOP_LEFT_CORNER), StepActionsLeft });

    StepWaitLeft->WaitTime = 19.0f; //  19
    auto RoamerLeftC = new Enemy({ ROAMER(OUT_TOP_LEFT_CORNER), StepActionsLeft });

    StepWaitLeft->WaitTime = 27.0f; //  27.0
    auto RoamerLeftD = new Enemy({ ROAMER(OUT_TOP_LEFT_CORNER), StepActionsLeft });

    std::vector<IStepAction*> StepActionsRight = {
        new StepWait(0.0f, false), // 0.0
        new StepMoveTowards(Vector2(Graphics::Get().GetScreenCenter().x + Size_256, Size_128)),
        new StepShoot(BehaviourType::EAimShot, 3, 0.5f),
        new StepMoveTowards(OUT_LOW_RIGHT),
        new StepMoveRelative(Vector2(+Size_512, 0)),
        new StepMoveTowards(Vector2(Graphics::Get().GetWindowArea().width, -Size_128)),
        new StepWait(1.0f)
    };
    auto RoamerRightA = new Enemy({ ROAMER(OUT_TOP_RIGHT_CORNER), StepActionsRight });

    StepWait* StepWaitRight = (StepWait*)StepActionsRight[0];
    StepWaitRight->WaitTime = 7.0f;    // 7.0
    auto RoamerRightB = new Enemy({ ROAMER(OUT_TOP_RIGHT_CORNER), StepActionsRight });

    StepWaitRight->WaitTime = 14.0f;    // 14.0
    auto RoamerRightC = new Enemy({ ROAMER(OUT_TOP_RIGHT_CORNER), StepActionsRight });

    StepWaitRight->WaitTime = 22.0f;    // 22
    auto RoamerRightD = new Enemy({ ROAMER(OUT_TOP_RIGHT_CORNER), StepActionsRight });

    std::vector<Enemy*> FirstSwarm = {
        RoamerLeftA, RoamerRightA,
        RoamerLeftB, RoamerRightB,
        RoamerLeftC, RoamerRightC,
        RoamerLeftD, RoamerRightD
    };

    AddWave(FirstSwarm);
}

void EnemyHandler::CreateThirdWave()
{
    std::vector<IStepAction*> RazorActions {
        new StepWait(1.0f),
        new StepMoveRelative(Vector2(0, Size_256)),
        new StepWait(1.0f, false),
        new StepEnterScene(POS_CENTER),
        new StepWait(1.0f),
        new StepShoot(BehaviourType::EHanaShot, 50, 0.25f),
        new StepWait(0.5f),
        new StepMoveTowards(POS_TOP_LEFT),
        new StepWait(0.5f),
        new StepShoot(BehaviourType::EHanaShot, 50, 0.25f),
        new StepWait(0.5f),
        new StepMoveTowards(POS_TOP_RIGHT),
        new StepShoot(BehaviourType::EHanaShot, 50, 0.25f),
        new StepWait(0.5f),
    };

    std::vector<IStepAction*> BungieActions{
        new StepWait(5.0f, false),
        new StepWait(1.0f),
        new StepSlideTowards(POS_LEFT),
        new StepShoot(BehaviourType::EAimShot, 5, 0.5f),
        new StepWait(0.25f),
        new StepMoveRelative(Vector2(0, -Size_256)),
        new StepWait(0.5f),
        new StepShoot(BehaviourType::EAimShot, 5, 0.5f),
        new StepWait(0.25f),
        new StepMoveRelative(Vector2(0, Size_512)),
    };

    auto BungieA = new Enemy( BUNGIE(Vector2(-Size_512, 750 * screenFactor)), BungieActions);
    delete BungieActions[2];
    BungieActions[2] = new StepSlideTowards(POS_RIGHT);
    auto BungieB = new Enemy( BUNGIE(Vector2(750 * screenFactor, 1280 * screenFactor)), BungieActions);

    std::vector<Enemy*> NextSwarm = { 
        new Enemy(RAZOR(Vector2(Graphics::Get().GetScreenCenter().x- Size_128, -Size_256)), RazorActions),
        BungieA,
        BungieB
    };

    AddWave(NextSwarm);
}

void EnemyHandler::CreateFourthWave()
{
    std::vector<IStepAction*> ObstacleActions{
        new StepWait(5.0f),
        new StepExit(300)
    };

    auto block_1 = new Enemy(BLOCK_A(OUT_TOP_CENTER), ObstacleActions);
    auto block_2 = new Enemy(BLOCK_B(Vector2Add(OUT_TOP_LEFT, Vector2(0, -64))), ObstacleActions);
    auto block_3 = new Enemy(BLOCK_C(Vector2Add(OUT_TOP_RIGHT, Vector2(0, -16))), ObstacleActions);

    StepWait* obstacleWaitTime = (StepWait*)ObstacleActions[0];
    obstacleWaitTime->WaitTime = 9.0f;
    StepExit* obstacleSpeed = (StepExit*)ObstacleActions[1];
    obstacleSpeed->Speed = 400;
    auto block_4 = new Enemy(BLOCK_E(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 9.0f;
    auto block_5 = new Enemy(BLOCK_D(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 12.0f;
    auto block_6 = new Enemy(CAR_A(OUT_TOP_CENTER), ObstacleActions);

    obstacleWaitTime->WaitTime = 15.0f;
    auto block_7 = new Enemy(BLOCK_B(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 18.0f;
    auto block_8 = new Enemy(CAR_B(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 20.0f;
    obstacleSpeed->Speed = 500;

    auto block_9 = new Enemy(BLOCK_C(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 24.0f;
    auto block_10 = new Enemy(BLOCK_B(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 28.0f;
    auto block_11 = new Enemy(CAR_B(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 30.0f;
    auto block_12 = new Enemy(BLOCK_E(OUT_TOP_CENTER), ObstacleActions);

    obstacleWaitTime->WaitTime = 32.0f;
    auto block_13 = new Enemy(BLOCK_E(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 36.0f;
    auto block_14 = new Enemy(BLOCK_D(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 48.0f;
    auto block_15 = new Enemy(CAR_C(OUT_TOP_LEFT), ObstacleActions);

    obstacleWaitTime->WaitTime = 51.0f;
    auto block_16 = new Enemy(BLOCK_C(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 56.0f;
    auto block_17 = new Enemy(CAR_A(OUT_TOP_CENTER), ObstacleActions);

    obstacleWaitTime->WaitTime = 59.0f;
    auto block_18 = new Enemy(CAR_C(OUT_TOP_CENTER), ObstacleActions);

    obstacleWaitTime->WaitTime = 64.0f;
    auto block_19 = new Enemy(BLOCK_E(OUT_TOP_RIGHT), ObstacleActions);

    obstacleWaitTime->WaitTime = 69.0f;
    auto block_20 = new Enemy(BLOCK_C(OUT_TOP_CENTER), ObstacleActions);

    /// ------------------------------------------------------------------ ///

    std::vector<IStepAction*> EnemiesActions{
        new StepWait(9.0f),
        new StepMoveRelative(Vector2(0, 512+256)),
        new StepShoot(BehaviourType::EAimShot, 5, 0.35f),
        new StepExit()
    };


    auto romaer_3 = new Enemy({ SWEPER(OUT_TOP_CENTER), EnemiesActions });

    StepWait * enemyWaitTime = (StepWait *)EnemiesActions[0];
    enemyWaitTime->WaitTime = 15.0f;
    auto romaer_1 = new Enemy({ SWEPER(OUT_TOP_LEFT_ROAD), EnemiesActions });
    enemyWaitTime->WaitTime = 16.0f;
    auto romaer_2 = new Enemy({ SWEPER(OUT_TOP_RIGHT_ROAD), EnemiesActions });




    std::vector<IStepAction*> customActions_A{
    new StepWait(23.f),
    new StepMoveRelative(Vector2(0, 512 + 256)),
    new StepShoot(BehaviourType::ERingShot, 2, 0.35f),
    new StepExit()
    };
    enemyWaitTime->WaitTime = 23.0f;
    auto romaer_4 = new Enemy({ SWEPER(OUT_TOP_LEFT_ROAD), customActions_A });

    enemyWaitTime->WaitTime = 27.0f;
    auto romaer_5 = new Enemy({ SWEPER(OUT_TOP_CENTER), EnemiesActions });

    enemyWaitTime->WaitTime = 30.0f;
    auto romaer_6 = new Enemy({ SWEPER(OUT_TOP_CENTER), EnemiesActions });

    enemyWaitTime->WaitTime = 36.0f;



    auto romaer_7 = new Enemy({ SWEPER(OUT_TOP_RIGHT_ROAD), EnemiesActions });

    enemyWaitTime->WaitTime = 42.0f;
    auto romaer_8 = new Enemy({ SWEPER(OUT_TOP_RIGHT_ROAD), EnemiesActions });


    std::vector<IStepAction*> customActions_B{
    new StepWait(36.0f),
    new StepMoveRelative(Vector2(0, 512 + 256)),
    new StepShoot(BehaviourType::EStarShot, 2, 0.35f),
    new StepExit()
    };
    enemyWaitTime->WaitTime = 55.0f;
    auto romaer_9 = new Enemy({ SWEPER(OUT_TOP_CENTER), EnemiesActions });


    enemyWaitTime->WaitTime = 56.0f;
    auto romaer_10 = new Enemy({ SWEPER(OUT_TOP_LEFT_ROAD), EnemiesActions });
    auto romaer_11 = new Enemy({ SWEPER(OUT_TOP_RIGHT_ROAD), customActions_B });

    std::vector<Enemy*> RushSwarm = {

        block_1, block_2, block_3, block_4, block_5, block_6, block_7, block_8,
        block_9, block_10, block_11, block_12, block_13, block_14, block_15, block_16, 
        block_17, block_18, block_19, block_20, 
        romaer_1, romaer_2, romaer_3, romaer_4, romaer_5, romaer_6, 
        romaer_7, romaer_8, romaer_9, romaer_10, romaer_11
        //new Enemy({ ROAMER(Vector2Add(OUT_TOP_RIGHT, Vector2(162, 0))), EnemiesActions })

    };

    AddWave(RushSwarm);
}

void EnemyHandler::CreateFifthWave()
{
    std::vector<IStepAction*> SeekerActions{
        //new StepWait(1.0f, false),          //
        new StepSlideTowards(POS_TOP_RIGHT),    // 0
        new StepShoot(BehaviourType::ESpiralShot, 20, 0.2f),
        new StepWait(0.25f),
        new StepSlideTowards(POS_LOW_RIGHT),    // 3
        new StepShoot(BehaviourType::ESpiralShot, 20, 0.2f),
        new StepWait(0.25f),
        new StepSlideTowards(POS_CENTER),       // 6
        new StepShoot(BehaviourType::EMultiSpiralShot, 30, 0.1f),
        new StepWait(0.25f),
        new StepSlideTowards(POS_LOW_LEFT),     // 9
        new StepShoot(BehaviourType::ESpiralShot, 20, 0.2f),
        new StepWait(0.25f),
        new StepSlideTowards(POS_TOP_LEFT),     // 12
        new StepShoot(BehaviourType::ESpiralShot, 20, 0.2f),
        new StepWait(0.25f),
    };

    EnemyProperties SeekerStats = SEEKER(OUT_TOP_RIGHT);
    auto SeekerA = new Enemy(SeekerStats, SeekerActions);
    SeekerStats.StartIndex = 6;
    auto SeekerB = new Enemy(SeekerStats, SeekerActions);
    SeekerStats.StartIndex = 12;
    auto SeekerC = new Enemy(SeekerStats, SeekerActions);

    std::vector<Enemy*> ThirdSwarm = {
        SeekerA,
        SeekerB,
        SeekerC
    };

    AddWave(ThirdSwarm);
}

void EnemyHandler::CreateSixthWave()
{
    std::vector<IStepAction*> VolboActions {
        new StepSlideTowards(Vector2(Graphics::Get().GetWindowArea().width - Size_64, Size_64)),      // 0 -
        new StepShoot(BehaviourType::ESeekShot, 1, 0.15f),
        new StepWait(0.25f),

        new StepSlideTowards(Vector2(64, Graphics::Get().GetWindowArea().height - Size_64)),     // 3 -
        new StepShoot(BehaviourType::ESeekShot, 1, 0.15f),
        new StepWait(0.25f),

        new StepSlideTowards(Vector2(Size_64, Size_64)),                                              // 6 -
        new StepShoot(BehaviourType::ESeekShot, 1, 0.15f),
        new StepWait(0.25f),

        new StepSlideTowards(Vector2(Graphics::Get().GetWindowArea().width - Size_64,
                                        Graphics::Get().GetWindowArea().height - Size_64)),      // 9 -
        new StepShoot(BehaviourType::ESeekShot, 1, 0.15f),
        new StepWait(0.25f),
    };


    std::vector<IStepAction*> VolboActionsSpecial{
        new StepSlideTowards(Vector2(Graphics::Get().GetWindowArea().width - Size_64, Size_64)),      // 0 -
        new StepWait(0.25f),

        new StepSlideTowards(POS_CENTER),                                                   // 2
        new StepShoot(BehaviourType::ERingShot, 3, 1.0f),
        new StepShoot(BehaviourType::EStarShot, 1, 0.1f),

        new StepSlideTowards(Vector2(Size_64, Graphics::Get().GetWindowArea().height - Size_64)),     // 5 -
        new StepWait(0.25f),

        new StepSlideTowards(Vector2(64, 64)),                                              // 7 -
        new StepWait(0.25f),

        new StepSlideTowards(Vector2(   Graphics::Get().GetWindowArea().width - Size_64,
                                        Graphics::Get().GetWindowArea().height - Size_64)),     // 9 -
        new StepWait(0.25f),
    };

    EnemyProperties VolboStats = VOLBO(OUT_TOP_RIGHT);
    auto VolboA = new Enemy(VolboStats, VolboActions);
    VolboStats.StartIndex = 3;
    auto VolboB = new Enemy(VolboStats, VolboActions);
    VolboStats.StartIndex = 6;
    auto VolboC = new Enemy(VolboStats, VolboActions);
    auto VolboD = new Enemy(VolboStats, VolboActionsSpecial);

    std::vector<Enemy*> ThirdSwarm = {
        VolboA,
        VolboB,
        VolboC,
        VolboD
    };

    AddWave(ThirdSwarm);
}

void EnemyHandler::CreateFinalWave()
{
    std::vector<IStepAction*> BossActions{
        new StepEnterScene(POS_CENTER),
        new StepWait(1.0f),
        new StepSlideTowards(POS_TOP_LEFT),
        new StepShoot(BehaviourType::EArchShot, 2, 0.15f),
        new StepWait(2.0f),
        new StepSlideTowards(POS_LOW_RIGHT),
        new StepShoot(BehaviourType::ESeekShot, 3, 0.25f),
        new StepWait(1.0f),
        new StepSlideTowards(POS_TOP_RIGHT),
        new StepWait(1.0f),
        new StepSlideTowards(POS_LOW_LEFT),
        new StepWait(1.0f),
        new StepSlideTowards(POS_TOP_LEFT),
        new StepShoot(BehaviourType::ESeekShot, 3, 0.25f),
        new StepSlideTowards(POS_LOW_RIGHT),
        new StepShoot(BehaviourType::EArchShot, 2, 0.15f),
        new StepWait(2.0f),
        new StepSlideTowards(POS_TOP_RIGHT),
        new StepWait(1.0f),
        new StepSlideTowards(POS_LOW_RIGHT),
        new StepShoot(BehaviourType::ESeekShot, 3, 0.25f),
        new StepSlideTowards(POS_TOP_RIGHT),
        new StepWait(1.0f),
        new StepSlideTowards(POS_LOW_LEFT),
        new StepShoot(BehaviourType::ESeekShot, 3, 0.25f),
    };

    std::vector<Enemy*> BossSwarm = {
        new Boss(BOSS(OUT_TOP_CENTER), BossActions)
    };

    AddWave(BossSwarm);
}


void EnemyHandler::AddWave(std::vector<Enemy*>& enemies)
{
    Swarms.push_back(new Swarm(enemies));
}

void EnemyHandler::Update()
{
    if (!Swarms.empty())
    {
        Swarms.front()->UpdateEnemies();

        if (checkWaveCompletion())  // Check if the current wave is complete
        {
            Swarms.front()->DeInit();
            Swarms.pop_front();     // Remove the completed wave

            SpawnCurrentWave();     // Spawn the next wave
        }; 
    }
}

bool EnemyHandler::checkWaveCompletion()
{
    return (Swarms.front() && Swarms.front()->AllEnemiesDestroyed());
}

void EnemyHandler::SpawnCurrentWave()
{
    if (!Swarms.empty())
    {
        CurrentWaveIndex++;
        Swarms.front()->SpawnEnemies();
    }
}

void EnemyHandler::Render()
{
    if (!Swarms.empty())
        for (Enemy* enemy : Swarms.front()->GetEnemies())
        {
	        if (enemy->Active)
		        enemy->Render();
        }
}

void EnemyHandler::Deinit()
{
    for (Swarm* swarm : Swarms)
    {
        delete swarm;
    }

    Swarms.clear();

    CurrentWaveIndex = 0;
}

const std::optional<std::vector<Enemy*>> EnemyHandler::GetCurrentWave()
{
    if (Swarms.empty() || Swarms.front()->GetEnemies().empty())
    {
        return std::nullopt;
    }
    return Swarms.front()->GetEnemies();  // If is not empty return elements
}
