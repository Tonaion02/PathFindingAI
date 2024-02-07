#include "EnemySystem.h"

#include "Data/Level/TileMap.h"

//Including some utils
#include "Misc/Direction.h"
//Including some utils

//Including Base ECS
#include "ECS/ECS.h"
//Including Base ECS

//Including components
#include "Components/Exploring/BaseEnemyComponent.h"
#include "Components/Exploring/MoveComponent.h"
//Including components

//Including systems
#include "Systems/Exploring/ActionSystem.h"
#include "Systems/Exploring/AnimationSystem.h"
#include "Systems/Exploring/MoveSystem.h"
//Including systems

//Including context
#include "Game.h"
#include "World.h"
//Including context





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class EnemySystem
//-----------------------------------------------------------------------------------------------------------------------------------------
void EnemySystem::init()
{
	World* world = Game::get()->getWorld();

	ComponentPool<BaseEnemyComponent>& BaseEnemyCmp = world->mPoolBaseEnemyComponent;
	ComponentPool<MoveComponent>& MoveCmp = world->mPoolMoveComponent;

	//Init all BaseEnemyComponent
	for (unsigned int i = 0; i < BaseEnemyCmp.mNext; i++)
	{
		BaseEnemyCmp.mPackedArray[i].alive = true;
		BaseEnemyCmp.mPackedArray[i].viewDistance = 3;
		BaseEnemyCmp.mPackedArray[i].currentStepPath = 0;
		MoveCmp.mPackedArray[i].lastDirection = BaseEnemyCmp.mPackedArray[i].path[0];
	}
	//Init all BaseEnemyComponent
}



void EnemySystem::aiBaseEnemy()
{
	World* world = Game::get()->getWorld();

	Level* currentLevel = &world->currentLevel;

	ComponentPool<BaseEnemyComponent>& BaseEnemyCmp = world->mPoolBaseEnemyComponent;
	ComponentPool<TransformComponent>& TransformCmp = world->mPoolTransformComponent;
	ComponentPool<ActionComponent>& ActionCmp = world->mPoolActionComponent;
	ComponentPool<MoveComponent>& MoveCmp = world->mPoolMoveComponent;

	for (unsigned int i = 0; i < BaseEnemyCmp.mNext; i++)
	{
		Entity e = world->mPoolBaseEnemyComponent.mDirectArray[i];

		Vector2i startPos = world->mPoolTransformComponent.mPackedArray[world->mPoolTransformComponent.mReverseArray[e]].tileOccupied;
		int z = world->mPoolTransformComponent.mPackedArray[world->mPoolTransformComponent.mReverseArray[e]].z;

		auto iter = std::find(world->battleEntities.begin(), world->battleEntities.end(), e);

		if (iter == world->battleEntities.end())
		{
			if (ActionSystem::isDoingNothing(e))
			{
				if (BaseEnemyCmp.mPackedArray[i].alive)
				{
					//Search the player beetween the range of view of the enemy
					bool found = false;

					


					//Search the player beetween the range of view of the enemy




					if (!found)
					{
						//Start an action if enemies not found the player
						ActionSystem::startAction(e, Actions::Walk);
						MoveSystem::startMove(e, BaseEnemyCmp.mPackedArray[i].path[BaseEnemyCmp.mPackedArray[i].currentStepPath]);
						AnimationSystem::startAnimation(e);
						BaseEnemyCmp.mPackedArray[i].currentStepPath++;


						//Controll if is terminated the path and reverse it
						if (BaseEnemyCmp.mPackedArray[i].currentStepPath == BaseEnemyCmp.mPackedArray[i].path.size())
							EnemySystem::reversePath(e);
						//Controll if is terminated the path and reverse it

						//Start an action if enemies not found the player
					}
				}
			}
		}
	}



	//If some enemy found an enemy
	if (!world->battleEntities.empty())
	{
		world->delayTransictionToBattle.timePassed += Game::get()->getDeltaTime();

		if (ActionSystem::isDoingNothing(world->player) && isEnd(world->delayTransictionToBattle))
		{
			//Before to startBattle try to delete allerting entities
			world->allertingEntities.clear();
			//Before to startBattle try to delete allerting entities


			//start battle
			Game::get()->startBattle();
		}
	}
	//If some enemy found an enemy
}



void EnemySystem::reversePath(Entity e)
{
	World* world = Game::get()->getWorld();

	ComponentPool<BaseEnemyComponent>& BaseEnemyCmp = world->mPoolBaseEnemyComponent;
	ComponentPool<MoveComponent>& MoveCmp = world->mPoolMoveComponent;

	std::vector<Direction> cpPath = BaseEnemyCmp.mPackedArray[BaseEnemyCmp.mReverseArray[e]].path;
	for (unsigned int i = 0; i < cpPath.size(); i++)
	{
		BaseEnemyCmp.mPackedArray[BaseEnemyCmp.mReverseArray[e]].path[i] = reverseDirection(cpPath[cpPath.size() - i - 1]);
	}

	BaseEnemyCmp.mPackedArray[BaseEnemyCmp.mReverseArray[e]].currentStepPath = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class EnemySystem
//-----------------------------------------------------------------------------------------------------------------------------------------