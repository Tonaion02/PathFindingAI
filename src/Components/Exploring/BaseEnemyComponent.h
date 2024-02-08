#pragma once

#include "Misc/Direction.h"

#include "utils/Math/Math.h"





struct BaseEnemyComponent
{
	bool alive;
	
	unsigned int viewDistance;
	
	
	std::vector<Direction> path;
	int currentStepPath;
};