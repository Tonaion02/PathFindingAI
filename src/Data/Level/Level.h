#pragma once

#include <string>
#include <vector>

#include "Math/Vector2i.h"





struct Level
{
public:
	static Level loadLevelFromFile(const std::string& filePath);
	static unsigned int retrieveIndexFromCoords(unsigned int x, unsigned int y, unsigned int z);

private:
	Level() {}

	static void initMap();

public:
	std::vector<short int> ids;
	Vector2i dim;
};