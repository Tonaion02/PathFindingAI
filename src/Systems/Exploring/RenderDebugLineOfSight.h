#pragma once

#include <vector>
#include <utility>

#include "SDL_pixels.h"

#include "utils/Math/Vector2i.h"





class RenderDebugLineOfSight
{
public:
	static void init();
	static void renderLines();
	static void addLine(const Vector2i& start, const Vector2i& end);

protected:
	static std::vector<std::pair<Vector2i, Vector2i>> linesToDraw;

	static const unsigned int reservedSpaceForLines;
	static const SDL_Color lineColor;
	static unsigned int nextLine;
};