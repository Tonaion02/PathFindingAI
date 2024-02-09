#pragma once

#include <array>

#include "utils/Math/Vector2i.h"





class PathFindingSystem
{
public:
	static void initGraph();
	static struct PathNode* findPath(const Vector2i& start, const Vector2i& end);
	static std::array<int, 4> dx;
	static std::array<int, 4> dy;
};