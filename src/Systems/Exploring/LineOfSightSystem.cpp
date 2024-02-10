#include "LineOfSightSystem.h"

#include "Game.h"
#include "World.h"

#include "Data/Level/Level.h"
#include "Data/Level/TileMap.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class LineOfSightSystem
//-----------------------------------------------------------------------------------------------------------------------------------------
bool LineOfSightSystem::isVisible(const Vector2i& start, const Vector2i& end)
{
	World* world = Game::get()->getWorld();
	Level* currentLevel = &world->currentLevel;



	//Assign to this variable start and end position
	float x1 = start.x, y1 = start.y;
	float x2 = end.x, y2 = end.y;
	//Assign to this variable start and end position

	float x, y, dx, dy, step;

	dx = (x2 - x1);
	dy = (y2 - y1);
	if (abs(dx) >= abs(dy))
		step = abs(dx);
	else
		step = abs(dy);
	dx = dx / step;
	dy = dy / step;
	x = x1;
	y = y1;
	int i = 0;
	while (i < step) {
		x = x + dx;
		y = y + dy;
		i = i + 1;
		
		if (Level::isInLevel(*currentLevel, x, y) && currentLevel->tileMap.tiles[x + y * currentLevel->dim.x].logicType == LogicType::Wall)
			return false;
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class LineOfSightSystem
//-----------------------------------------------------------------------------------------------------------------------------------------