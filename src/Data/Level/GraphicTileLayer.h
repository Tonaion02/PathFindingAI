#pragma once

#include <array>
#include <vector>





#define MAX_W_MAP 160
#define MAX_H_MAP 160
#define MAX_Z_MAP 6





template<std::size_t n>
struct GraphicTileLayer
{
	GraphicTileLayer() {
		gTiles.resize(n);
	}

	std::vector<short int> gTiles;
	std::vector<int> deepOfLayer;
};