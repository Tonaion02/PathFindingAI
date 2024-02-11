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
#include "Systems/Exploring/PathFindingSystem.h"
#include "Systems/Exploring/LineOfSightSystem.h"
//Including systems

//Including debug's systems
#include "Systems/Exploring/RenderDebugLineOfSight.h"
//Including debug's systems

//Including context
#include "Game.h"
#include "World.h"
//Including context





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class EnemySystem
//-----------------------------------------------------------------------------------------------------------------------------------------
const unsigned int EnemySystem::angleView = 110;
PathFindingSystem::MakeEstimation* EnemySystem::euristic;



void EnemySystem::init()
{
	World* world = Game::get()->getWorld();

	euristic = world->euclidean;

	ComponentPool<BaseEnemyComponent>& BaseEnemyCmp = world->mPoolBaseEnemyComponent;
	ComponentPool<MoveComponent>& MoveCmp = world->mPoolMoveComponent;

	//Init all BaseEnemyComponent
	for (unsigned int i = 0; i < BaseEnemyCmp.mNext; i++)
	{
		BaseEnemyCmp.mPackedArray[i].alive = true;
		BaseEnemyCmp.mPackedArray[i].viewDistance = 5;
		BaseEnemyCmp.mPackedArray[i].currentStepPath = 0;
		MoveCmp.mPackedArray[i].lastDirection = BaseEnemyCmp.mPackedArray[i].path[0];

		BaseEnemyCmp.mPackedArray[i].lastPosPlayer = { -1, -1 };
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
				if (BaseEnemyCmp.mPackedArray[i].lastPosPlayer == startPos)
				{
					BaseEnemyCmp.mPackedArray[i].lastPosPlayer = { -1, -1 };
				}

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

					//On base of direction set initial value for initHp and initBp
					//initHp and initBp describes the two coords of center of base of triangle
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
					//On base of direction set initial value for initHp and initBp

					//Set initial value for sbp and ebp
					//sbp: describes the starting coord for the base
					//sbp: describes the ending coord for the base
					int sbp, ebp;
					sbp = initBp - halfTriangleBase;
					ebp = initBp + halfTriangleBase;
					//Set initial value for sbp and ebp

					//Set sign on the base of direction
					//sign: is used to sum or subtract progressive deep of fieldOfView on the base of direction
					int sign = -1;
					if (currentDirection == Direction::Down)
						sign = -1;
					else if (currentDirection == Direction::Up)
						sign = 1;
					else if (currentDirection == Direction::Right)
						sign = -1;
					else if (currentDirection == Direction::Left)
						sign = 1;
					//Set sign on the base of direction



					Vector2i p;
					//Loop on progressive deep of field of view to search the player
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
								RenderDebugLineOfSight::addLine(startPos, p);
								SDL_Log("Sei visibile!!!");
							}

							if (Level::isInLevel(*currentLevel, p.x, p.y, z) && 
								currentLevel->tileMap.mappedEntities[z * currentLevel->dim.x * currentLevel->dim.y + p.y * currentLevel->dim.x + p.x]
								== EntityOccupier::PlayerOccupier
								&& LineOfSightSystem::isVisible(startPos, p) )
							{
								found = true;
								BaseEnemyCmp.mPackedArray[i].lastPosPlayer = p;
								
								break;
							}

							s += 1;
						}

						//If found the player, is useless to check other positions
						if (found == true)
							break;
							

						//Until sbp and ebp(starting point and ending point of base of triangle) is the same, restrict the base
						if (sbp != ebp)
						{
							sbp += 1;
							ebp -= 1;
						}
						//Until sbp and ebp(starting point and ending point of base of triangle) is the same, restrict the base
					}
					//Loop on progressive deep of field of view to search the player


					//Search the player beetween the range of view of the enemy



					//If see the player
					Vector2i lastPosPlayer = getCmpEntity(BaseEnemyCmp, e).lastPosPlayer;
					if (found || (lastPosPlayer.x != -1 && lastPosPlayer.y != -1)) 
					{
						//SDL_Log("TROVATO PORCODIO");

						PathNode* pathNode = PathFindingSystem::findPath(startPos, lastPosPlayer, euristic);
						if (pathNode != nullptr && pathNode->parent != nullptr)
						{
							Vector2i nextPos = pathNode->parent->pos;
							


							Direction nextDirection = Direction::NoneDirection;
							Vector2i diff = nextPos - startPos;
							if (diff.x == 0)
							{
								if (diff.y == 1)
									nextDirection = Direction::Down;
								else
									nextDirection = Direction::Up;
							}
							else
							{
								if (diff.x == 1)
									nextDirection = Direction::Right;
								else
									nextDirection = Direction::Left;
							}
							


							ASSERT(nextDirection != Direction::NoneDirection)



							ActionSystem::startAction(e, Actions::Walk);
							MoveSystem::startMove(e, nextDirection);
							AnimationSystem::startAnimation(e);
						}
						else
						{
							BaseEnemyCmp.mPackedArray[i].lastPosPlayer = { -1, -1 };
						}
					}
					//If see the player



					//If can't see the player, idle logic
					if (!found && lastPosPlayer.x == -1 && lastPosPlayer.y == -1)
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



			if (BaseEnemyCmp.mPackedArray[i].lastPosPlayer.x != -1 && BaseEnemyCmp.mPackedArray[i].lastPosPlayer.y != -1)
			{
				RenderDebugLineOfSight::addLine(startPos, BaseEnemyCmp.mPackedArray[i].lastPosPlayer);
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