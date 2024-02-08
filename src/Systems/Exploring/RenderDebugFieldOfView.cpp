#include "RenderDebugFieldOfView.h"

#include "ECS/ECS.h"

#include "Game.h"

#include "World.h"

#include "Enviroment/WindowHandler.h"

#include "Data/Camera/Camera.h"

#include "Data/Level/Level.h"

#include "Systems/Exploring/EnemySystem.h"
#include "Systems/Exploring/ActionSystem.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class RenderDebugFieldOfViewSystem
//-----------------------------------------------------------------------------------------------------------------------------------------
const SDL_Color RenderDebugFieldOfView::color = { 255, 0, 0, 150 };



void RenderDebugFieldOfView::render()
{
	World* world = Game::get()->getWorld();



	SDL_SetRenderDrawBlendMode(WindowHandler::get().getRenderer(), SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(WindowHandler::get().getRenderer(), color.r, color.g, color.b, color.a);



	Level* currentLevel = &world->currentLevel;

	float baseScale = world->cameraData.baseScale;
	Vector2i adj = world->cameraData.adj;

	int tileDim = static_cast<int>(currentLevel->tileSet->tileDim.x * baseScale);

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
			if (BaseEnemyCmp.mPackedArray[i].alive)
			{
				//Render all the tile where the player is searched
				unsigned int viewDistance = getCmpEntity(BaseEnemyCmp, e).viewDistance;

				//Compute triangle's base of view
				float angle = 90.0f - (EnemySystem::angleView / 2.0f);
				float rad = ToRadians(angle);
				float t = tanf(ToRadians(angle));
				float triangleBasef = viewDistance / abs(tanf(ToRadians(angle)));
				triangleBasef *= 2.0f;
				triangleBasef = roundf(triangleBasef);
				unsigned int triangleBase = static_cast<unsigned int>(triangleBasef);
				if (triangleBase % 2 == 0)
					triangleBase -= 1;
				//Compute triangle's base of view

				//Compute half of triangle base
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

				for (unsigned int d = 0; d < viewDistance; d++)
				{
					int hp = initHp + d * sign;

					int s, e;
					s = sbp;
					e = ebp;
					SDL_Rect rect;
					rect.w = tileDim;
					rect.h = tileDim;
					/*rect.y = hp * tileDim;*/
					while (s != e + 1)
					{
						//rect.x = s * tileDim;

						if (currentDirection == Direction::Down)
						{
							rect.x = s * tileDim;
							rect.y = hp * tileDim;
						}
						else if (currentDirection == Direction::Up)
						{
							rect.x = s * tileDim;
							rect.y = hp * tileDim;
						}
						else if (currentDirection == Direction::Right)
						{
							rect.y = s * tileDim;
							rect.x = hp * tileDim;
						}
						else if (currentDirection == Direction::Left)
						{
							rect.y = s * tileDim;
							rect.x = hp * tileDim;
						}

						rect.x += adj.x;
						rect.y += adj.y;

						SDL_RenderFillRect(WindowHandler::get().getRenderer(), &rect);

						s += 1;
					}

					if (sbp != ebp)
					{
						sbp += 1;
						ebp -= 1;
					}
				}

				//Only down direction
				//unsigned int halfTriangleBase = (triangleBase - 1) / 2;
				//Vector2i centerOfTriangleBase = { startPos.x, startPos.y + static_cast<int>(viewDistance) };
				//Vector2i startBasePos = centerOfTriangleBase;
				//startBasePos.x -= halfTriangleBase;
				//Vector2i endBasePos = centerOfTriangleBase;
				//endBasePos.x += halfTriangleBase;
				


				//SDL_Rect rect;
				//rect.w = tileDim;
				//rect.h = tileDim;
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
				//	rect.y = s.y * tileDim;
				//	while (s.x != e.x+1) {
				//		rect.x = s.x * tileDim;

				//		SDL_RenderFillRect(WindowHandler::get().getRenderer(), &rect);

				//		s.x += 1;
				//	}
				//}
				//Only down direction







				//Render all the tile where the player is searched 
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//Class RenderDebugFieldOfViewSystem
//-----------------------------------------------------------------------------------------------------------------------------------------