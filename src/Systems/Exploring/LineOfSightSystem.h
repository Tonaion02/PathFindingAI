#pragma once

#include "utils/Math/Vector2i.h"





class LineOfSightSystem
{
public:
	static bool isVisible(const Vector2i& start, const Vector2i& end);
};