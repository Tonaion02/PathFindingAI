#pragma once

#include "utils/Math/Vector2i.h"





struct PathNode
{
	//Coords of tile represented by node
	Vector2i pos;
	//Cost to arrive from current node to end node H(n)
	float estimation;
	//Cost to arrive from starting node to current node G(n)
	float cost;
	//Father of actual node, nullptr in case starting node
	struct PathNode* parent;



	bool operator==(const PathNode& pathNode) const
	{
		return pathNode.pos.x == this->pos.x && pathNode.pos.y == this->pos.y;
	}

	bool operator>(const PathNode& other) const
	{
		return this->cost + this->estimation > other.cost + other.estimation;
	}
};