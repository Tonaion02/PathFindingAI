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

				//Loop on progessive deep of field
				//The progressive deep of field is reppresented by d variable
				for (unsigned int d = 0; d < viewDistance; d++)
				{
					int hp = initHp + d * sign;

					int s, e;
					s = sbp;
					e = ebp;
					SDL_Rect rect;
					rect.w = tileDim;
					rect.h = tileDim;
					while (s != e + 1)
					{
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

					//Until sbp and ebp(starting point and ending point of base of triangle) is the same, restrict the base
					if (sbp != ebp)
					{
						sbp += 1;
						ebp -= 1;
					}
					//Until sbp and ebp(starting point and ending point of base of triangle) is the same, restrict the base
				}
				//Loop on progessive deep of field


				//Render all the tile where the player is searched 
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//Class RenderDebugFieldOfViewSystem
//-----------------------------------------------------------------------------------------------------------------------------------------