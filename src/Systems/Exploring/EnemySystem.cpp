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
const unsigned int EnemySystem::angleView = 110;



void EnemySystem::init()
{
	World* world = Game::get()->getWorld();

	ComponentPool<BaseEnemyComponent>& BaseEnemyCmp = world->mPoolBaseEnemyComponent;
	ComponentPool<MoveComponent>& MoveCmp = world->mPoolMoveComponent;

	//Init all BaseEnemyComponent
	for (unsigned int i = 0; i < BaseEnemyCmp.mNext; i++)
	{
		BaseEnemyCmp.mPackedArray[i].alive = true;
		BaseEnemyCmp.mPackedArray[i].viewDistance = 5;
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

					//Retrieve viewDistance
					unsigned int viewDistance = getCmpEntity(BaseEnemyCmp, e).viewDistance;

					//Compute triangleBase
					unsigned int triangleBase = computeTriangleBase(viewDistance);
					
					//Compute half of triangleBase
					unsigned int halfTriangleBase = (triangleBase - 1) / 2;

					//Retrieve direction of player
					Direction currentDirection = (Direction)getCmpEntity(MoveCmp, e).currentDirection;
					if (currentDirection == Direction::NoneDirection)
						currentDirection = (Direction)getCmpEntity(MoveCmp, e).lastDirection;
					//Retrieve direction of player

					int initHp, initBp;
					if (currentDirection == Direction::Down)
					{
						initHp = startPos.y + viewDistance;
						initBp = startPos.x;
					}
					else if (currentDirection == Direction::Up)
					{
						initHp = startPos.y - viewDistance;
						initBp = startPos.x;
					}
					else if (currentDirection == Direction::Right)
					{
						initHp = startPos.x + viewDistance;
						initBp = startPos.y;
					}
					else if (currentDirection == Direction::Left)
					{
						initHp = startPos.x - viewDistance;
						initBp = startPos.y;
					}

					int sbp, ebp;
					sbp = initBp - halfTriangleBase;
					ebp = initBp + halfTriangleBase;


					int sign = -1;
					if (currentDirection == Direction::Down)
						sign = -1;
					else if (currentDirection == Direction::Up)
						sign = 1;
					else if (currentDirection == Direction::Right)
						sign = -1;
					else if (currentDirection == Direction::Left)
						sign = 1;

					Vector2i p;
					for (unsigned int d = 0; d < viewDistance; d++)
					{
						int hp = initHp + d * sign;

						int s, e;
						s = sbp;
						e = ebp;
						while (s != e + 1)
						{
							if (currentDirection == Direction::Down)
							{
								p = { s, hp };
							}
							else if (currentDirection == Direction::Up)
							{
								p = { s, hp };
							}
							else if (currentDirection == Direction::Right)
							{
								p = { hp, s };
							}
							else if (currentDirection == Direction::Left)
							{
								p = { hp, s };
							}

							if (Level::isInLevel(*currentLevel, p.x, p.y, z) && 
								currentLevel->tileMap.mappedEntities[z * currentLevel->dim.x * currentLevel->dim.y + p.y * currentLevel->dim.x + p.x]
								== EntityOccupier::PlayerOccupier)
							{
								found = true;
								break;
							}

							s += 1;
						}

						if (found == true)
							break;

						if (sbp != ebp)
						{
							sbp += 1;
							ebp -= 1;
						}
					}






					////Only down direction
					//unsigned int halfTriangleBase = (triangleBase - 1) / 2;
					//Vector2i centerOfTriangleBase = { startPos.x, startPos.y + static_cast<int>(viewDistance) };
					//Vector2i startBasePos = centerOfTriangleBase;
					//startBasePos.x -= halfTriangleBase;
					//Vector2i endBasePos = centerOfTriangleBase;
					//endBasePos.x += halfTriangleBase;
					//



					//for (unsigned int d = 0; d < viewDistance; d++)
					//{
					//	if (startBasePos.x != endBasePos.x)
					//	{
					//		startBasePos.x += 1;
					//		endBasePos.x -= 1;
					//	}

					//	Vector2i s, e;
					//	s = startBasePos;
					//	s.y -= d;
					//	e = endBasePos;
					//	e.y -= d;
					//	while (s.x != e.x + 1) {

					//		if (currentLevel->tileMap.mappedEntities[z * currentLevel->dim.x * currentLevel->dim.y + s.y * currentLevel->dim.x + s.x]
					//			== EntityOccupier::PlayerOccupier) 
					//			found = true;

					//		s.x += 1;
					//	}
					//}
					////Only down direction

					//Search the player beetween the range of view of the enemy



					//If see the player
					if (found)
						SDL_Log("TROVATO PORCODIO");
					//If see the player



					//If can't see the player, idle logic
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
					//If can't see the player, idle logic
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



unsigned int EnemySystem::computeTriangleBase(int viewDistance)
{
	float angle = 90.0f - (EnemySystem::angleView / 2.0f);
	float rad = ToRadians(angle);
	float t = tanf(ToRadians(angle));
	float triangleBasef = viewDistance / abs(tanf(ToRadians(angle)));
	triangleBasef *= 2.0f;
	triangleBasef = roundf(triangleBasef);
	unsigned int triangleBase = static_cast<unsigned int>(triangleBasef);
	if (triangleBase % 2 == 0)
		triangleBase -= 1;

	return triangleBase;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class EnemySystem
//-----------------------------------------------------------------------------------------------------------------------------------------