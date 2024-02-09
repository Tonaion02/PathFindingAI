#include "PathFindingSystem.h"

#include <string>

#include <functional>

#include <queue>
#include "Data/Level/MyQueue.h"

//Include game context
#include "Game.h"
#include "World.h"
//Include game context

//Include representation of graph
#include "Data/Level/Graph.h"
//Include representation of graph





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PathFindingSystem
//-----------------------------------------------------------------------------------------------------------------------------------------
struct ComparePathNode
{
	bool operator()(PathNode* l, PathNode* r)
	{
		return l->cost + l->estimation > r->cost + r->estimation;
	}
};




std::array<int, 4> PathFindingSystem::dx = { -1, 0, 1, 0 };
std::array<int, 4> PathFindingSystem::dy = { 0, 1, 0, -1 };



void PathFindingSystem::initGraph()
{
	World* world = Game::get()->getWorld();

	world->graph.nextNode = 0;
	world->graph.pathNodes.resize(MAX_H_MAP * MAX_W_MAP);
	world->graph.visited.resize(MAX_H_MAP * MAX_W_MAP);
}



PathNode* PathFindingSystem::findPath(const Vector2i& start, const Vector2i& end)
{
	World* world = Game::get()->getWorld();
	Graph& graph = world->graph;
	Level& currentLevel = world->currentLevel;



	graph.nextNode = 0;
	std::fill(world->graph.visited.begin(), world->graph.visited.end(), false);



	MyQueue<PathNode*, std::vector<PathNode*>, ComparePathNode> minHeap;
	PathNode& pathNode = graph.pathNodes[graph.nextNode];
	
	pathNode.cost = 0.0f;
	pathNode.estimation = 0.0f;
	pathNode.parent = nullptr;
	pathNode.pos = start;
	minHeap.emplace(&pathNode);
	
	graph.nextNode++;
	PathNode& testNode = graph.pathNodes[graph.nextNode];
	testNode.cost = 0.0f;
	testNode.estimation = 1.0f;
	testNode.pos = start;
	testNode.pos.x += 1;
	minHeap.emplace(&testNode);

	graph.nextNode++;
	PathNode& testNode2 = graph.pathNodes[graph.nextNode];
	testNode2.cost = 0.0f;
	testNode2.estimation = 2.0f;
	testNode2.pos = start;
	testNode2.pos.y += 1;
	minHeap.emplace(&testNode2);

	graph.nextNode++;
	

	while (!minHeap.empty())
	{
		
		//PathNode* currentNode = minHeap.top();
		//minHeap.pop();

		//PathNode* testNode = minHeap.top();
		//minHeap.pop();

		//PathNode* testNode2p = minHeap.top();
		//minHeap.pop();

		//SDL_Log(std::to_string(*currentNode > *testNode).c_str());

		PathNode n2;
		n2.pos = start;
		n2.pos.x += 1;
		PathNode* p = *minHeap.find(&n2);

		int r = 0;

		//PathNode* testNode = new PathNode();
		//testNode->cost = 0.0f;
		//testNode->estimation = 1.0f;

		//SDL_Log(std::to_string(*currentNode > *testNode).c_str());




		//if (currentNode.pos.x == end.x && currentNode.pos.y == end.y)
		//{

		//}

		//graph.visited[currentNode.pos.x + currentNode.pos.y * world->currentLevel.dim.x] = true;



		//Vector2i nextNodePos;
		//for (int i = 0; i < dx.size(); i++)
		//{
		//	nextNodePos.x = currentNode.pos.x + dx[i];
		//	nextNodePos.y = currentNode.pos.y + dy[i];

		//	if (Level::isInLevel(currentLevel, nextNodePos.x, nextNodePos.y) == false
		//		|| currentLevel.tileMap.tiles[currentLevel.dim.x * nextNodePos.y + nextNodePos.x].logicType == LogicType::Wall)
		//		continue;

			//if (graph.visited[nextNodePos.x + nextNodePos.y * world->currentLevel.dim.x] == true)
			//{
			//	PathNode nextNode;
			//	nextNode.pos = nextNodePos;
			//	auto actualNode = *minHeap.find(nextNode);

			//	nextNode.cost = currentNode.cost + 1;
			//	if (nextNode.cost < actualNode.cost)
			//		actualNode.parent = &currentNode;
			//}
		//}
	}









	graph.nextNode = 0;

	return nullptr;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PathFindingSystem
//-----------------------------------------------------------------------------------------------------------------------------------------