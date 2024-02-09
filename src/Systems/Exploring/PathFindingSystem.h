#pragma once

#include <array>

#include "utils/Math/Vector2i.h"





class PathFindingSystem
{
public:
	class MakeEstimation
	{
	public:
		virtual float operator()(Vector2i currentPos, Vector2i end) const = 0;
	};

	static void initGraph();
	static struct PathNode* findPath(const Vector2i& start, const Vector2i& end, MakeEstimation* estimation);
	static std::array<int, 4> dx;
	static std::array<int, 4> dy;
};