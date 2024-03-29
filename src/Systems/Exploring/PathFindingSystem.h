#pragma once

#include <array>

#include "utils/Math/Vector2i.h"





class PathFindingSystem
{
public:
	class MakeEstimation
	{
	public:
		virtual float operator()(const Vector2i& currentPos, const Vector2i& end) const = 0;
	};

	class EuclideanDistance : public PathFindingSystem::MakeEstimation
	{
	public:
		virtual float operator()(const Vector2i& currentPos, const Vector2i& end) const override
		{
			return sqrtf(((currentPos.x - end.x) * (currentPos.x - end.x)) + ((currentPos.y - end.y) * (currentPos.y - end.y))) * PathFindingSystem::weight;
		}
	};

	class ManhattanDistance : public PathFindingSystem::MakeEstimation
	{
	public:
		virtual float operator()(const Vector2i& currentPos, const Vector2i& end) const override
		{
			return (abs(currentPos.x - end.x) + abs(currentPos.y - end.y))  * PathFindingSystem::weight;
		}
	};

	class ChebyShevDistance : public PathFindingSystem::MakeEstimation
	{
	public:
		virtual float operator()(const Vector2i& currentPos, const Vector2i& end) const override
		{
			return std::max(std::abs(currentPos.x - end.x), std::abs(currentPos.y - end.y)) * PathFindingSystem::weight;
		}
	};

	static void initGraph();
	static struct PathNode* findPath(const Vector2i& start, const Vector2i& end, MakeEstimation* estimation);
	
	static unsigned int getExploredNodesCounter()
	{
		return exploredNodesCounter;
	}
protected:
	static std::array<int, 4> dx;
	static std::array<int, 4> dy;

private:
	static unsigned int exploredNodesCounter;
	static const float weight;
};