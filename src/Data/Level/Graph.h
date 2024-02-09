#pragma once

#include <vector>

#include "PathNode.h"





struct Graph
{
	std::vector<PathNode> pathNodes;
	unsigned int nextNode;
	std::vector<bool> visited;
};