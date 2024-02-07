#pragma once

#include <string>
#include <vector>

#include "Math/Vector2i.h"
#include "Data/Level/GraphicTileLayer.h"

#include "Data/Level/TileMap.h"





struct Level
{
public:
	static Level* loadLevelFromFile(const std::string& filePath);
	static unsigned int retrieveIndexFromCoords(unsigned int x, unsigned int y, unsigned int z);

private:
	Level() {}

	static void initMap();

public:
	Vector2i dim;
	int maxZ;

	TileMap tileMap;
	GraphicTileLayer<MAX_W_MAP * MAX_H_MAP * MAX_Z_MAP> graphicTileLayer;
};