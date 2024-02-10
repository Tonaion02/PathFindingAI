#include "RenderDebugLineOfSight.h"

//Including Game context
#include "Game.h"
#include "World.h"
//Including Game context

#include "Data/Camera/Camera.h"
#include "Data/Level/Level.h"

//Including some enviroments
#include "Enviroment/WindowHandler.h"
//Including some enviroments





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class RenderDebugLineOfSight
//-----------------------------------------------------------------------------------------------------------------------------------------
std::vector<std::pair<Vector2i, Vector2i>> RenderDebugLineOfSight::linesToDraw;
unsigned int RenderDebugLineOfSight::nextLine = 0;

const unsigned int RenderDebugLineOfSight::reservedSpaceForLines = 40;
const SDL_Color RenderDebugLineOfSight::lineColor = {0, 255, 0, 255};



void RenderDebugLineOfSight::init()
{
	linesToDraw.resize(RenderDebugLineOfSight::reservedSpaceForLines);
}



void RenderDebugLineOfSight::renderLines()
{
	World* world = Game::get()->getWorld();
	
	Level* currentLevel = &world->currentLevel;
	
	Vector2i adj = world->cameraData.adj;
	float baseScale = world->cameraData.baseScale;

	int tileDim = static_cast<int>(currentLevel->tileSet->tileDim.x * baseScale);



	SDL_SetRenderDrawBlendMode(WindowHandler::get().getRenderer(), SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(WindowHandler::get().getRenderer(), lineColor.r, lineColor.g, lineColor.b, lineColor.a);



	//Draw all the lines
	for (unsigned int i = 0;i < nextLine; i++)
	{
		auto points = linesToDraw[i];

		points.first.x = static_cast<int>((points.first.x + 0.5f)* tileDim + adj.x);
		points.first.y = static_cast<int>((points.first.y + 0.5f)* tileDim + adj.y);
		points.second.x = static_cast<int>((points.second.x + 0.5f)* tileDim + adj.x);
		points.second.y = static_cast<int>((points.second.y + 0.5f)* tileDim + adj.y);

		SDL_RenderDrawLine(WindowHandler::get().getRenderer(), points.first.x, points.first.y, points.second.x, points.second.y);
	}
	//Draw all the lines

	
	nextLine = 0;
}



void RenderDebugLineOfSight::addLine(const Vector2i & start, const Vector2i & end)
{
	linesToDraw[nextLine] = std::pair<Vector2i, Vector2i>(start, end);
	nextLine++;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class RenderDebugLineOfSight
//-----------------------------------------------------------------------------------------------------------------------------------------