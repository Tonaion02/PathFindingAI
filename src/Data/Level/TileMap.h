#pragma once

#include <vector>





enum LogicType
{
	NoneLogicType = -1,
	Ground,
	Wall,
	Sea,
	Chest
};


enum EntityOccupier
{
	NoneEntityOccupier = -1,
	PlayerOccupier,
	EnemyOccupier
};





struct Tile
{
	short int logicType = LogicType::NoneLogicType;
	bool visible = false;
};






struct TileMap
{
	std::vector<Tile> tiles;
	std::vector<int> mappedEntities;
};
