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
std::array<int, 4> PathFindingSystem::dx = { -1, 0, 1, 0 };
std::array<int, 4> PathFindingSystem::dy = { 0, 1, 0, -1 };

unsigned int PathFindingSystem::exploredNodesCounter = 0;
const float PathFindingSystem::weight = 2.0f;





class MinHeap
{
public:

	MinHeap(unsigned int size)
	{
		heap.reserve(size);
	}

	void insert(PathNode* val) {
		heap.push_back(val);
		heapifyUp(heap.size() - 1);
	}

	PathNode* pop() {
		if (heap.empty()) {
			return nullptr;
		}
		PathNode* minVal = heap[0];
		heap[0] = heap.back();
		heap.pop_back();
		heapifyDown(0);
		return minVal;
	}

	PathNode* top() const {
		if (heap.empty()) {
			return nullptr; 
		}
		return heap[0];
	}

	bool empty() const {
		return heap.empty();
	}

	size_t size() const {
		return heap.size();
	}

	void updateKey(int index, PathNode* newValue) {
		if (index >= heap.size()) {
			return;
		}

		if (*heap[index] > *newValue)
		{
			*heap[index] = *newValue;
			if (index > 0 && *heap[index] < *heap[(index - 1) / 2])
				heapifyUp(index);
			else
				heapifyDown(index);
		}
	}

	PathNode* find(PathNode* val) {
		return *std::find_if(heap.begin(), heap.end(), [&val](const PathNode* p) { return p->pos.x == val->pos.x && p->pos.y == val->pos.y; });
	}

	int find(Vector2i pos) {
		auto iter = std::find_if(heap.begin(), heap.end(), [&pos](const PathNode* p) { return p->pos.x == pos.x && p->pos.y == pos.y; });

		if (iter == heap.end())
			return -1;
		return iter - heap.begin();
	}

private:
	void heapifyUp(int index) {
		int parent = (index - 1) / 2;
		while (index > 0 && *heap[index] < *heap[parent]) {
			std::swap(heap[index], heap[parent]);
			index = parent;
			parent = (index - 1) / 2;
		}
	}

	void heapifyDown(int index) {
		int leftChild, rightChild, minChild;
		while (2 * index + 1 < heap.size()) {
			leftChild = 2 * index + 1;
			rightChild = 2 * index + 2;
			minChild = leftChild;
			if (rightChild < heap.size() && *heap[rightChild] < *heap[leftChild])
				minChild = rightChild;
			if (*heap[index] > *heap[minChild]) {
				std::swap(heap[index], heap[minChild]);
				index = minChild;
			}
			else {
				break;
			}
		}
	}

private:
	std::vector<PathNode*> heap;
};



void PathFindingSystem::initGraph()
{
	World* world = Game::get()->getWorld();

	//Set size of graph and explored matrix
	world->graph.nextNode = 0;
	world->graph.pathNodes.resize(MAX_H_MAP * MAX_W_MAP);
	//std::fill(world->graph.pathNodes.begin(), world->graph.pathNodes.end(), { {-1, -1}, 0.0f, 0.0f, nullptr });
	for (PathNode& node : world->graph.pathNodes)
	{
		node.pos = { -1, -1 };
		node.cost = 0.0f;
		node.estimation = 0.0f;
		node.parent = nullptr;
	}
	world->graph.visited.resize(MAX_H_MAP * MAX_W_MAP);
	//Set size of graph and explored matrix
}



PathNode* PathFindingSystem::findPath(const Vector2i& start, const Vector2i& end, MakeEstimation* estimation)
{
	World* world = Game::get()->getWorld();
	Graph& graph = world->graph;
	Level& currentLevel = world->currentLevel;



	//Check if the tile where you start path finding is a valid position
	ASSERT(Level::isInLevel(currentLevel, start.x, start.y));
	//Check if the tile where you start path finding is a valid position

	//Check if the tile where you start path finding is a walkable tile
	ASSERT(currentLevel.tileMap.tiles[start.x + start.y * currentLevel.dim.x].logicType != LogicType::Wall);
	//Check if the tile where you start path finding is a walkable tile

	//Check if the destination is a valid position
	ASSERT(Level::isInLevel(currentLevel, end.x, end.y));
	//Check if the destination is a valid position

	//Check if the destination is walkable 
	if (currentLevel.tileMap.tiles[end.x + end.y * currentLevel.dim.x].logicType == LogicType::Wall)
		return nullptr;
	//Check if the destination is walkable



	//Reset graph struct
	graph.nextNode = 0;
	std::fill(world->graph.visited.begin(), world->graph.visited.end(), false);
	//Reset graph struct

	//Testing
	PathFindingSystem::exploredNodesCounter = 0;
	//Testing



	//Create MinHeap
	MinHeap minHeap(MAX_H_MAP * MAX_W_MAP);
	//Create MinHeap
	
	//Create first node to explore and add to heap
	PathNode& pathNode = graph.pathNodes[graph.nextNode];
	pathNode.cost = 0.0f;
	pathNode.estimation = (*estimation)(end, start);
	pathNode.parent = nullptr;
	pathNode.pos = end;
	minHeap.insert(&pathNode);
	graph.nextNode++;
	//Create first node to explore and add to heap



	//Until the heap is empty loop
	while (!minHeap.empty())
	{
		//Retrieve node on top of heap and pop from heap that node
		PathNode& currentNode = *minHeap.top();
		
		minHeap.pop();
		//Retrieve node on top of heap and pop from heap that node

		//Check if you reach the end position, in case return the current node
		if (currentNode.pos.x == start.x && currentNode.pos.y == start.y)
		{
			return &currentNode;
		}
		//Check if you reach the end position, in case return the current node

		//Set to explored the current node
		graph.visited[currentNode.pos.x + currentNode.pos.y * world->currentLevel.dim.x] = true;
		//Set to explored the current node



		Vector2i nextNodePos;
		for (int i = 0; i < dx.size(); i++)
		{
			//Compute the position of the nextNode to evalue
			nextNodePos.x = currentNode.pos.x + dx[i];
			nextNodePos.y = currentNode.pos.y + dy[i];
			//Compute the position of the nextNode to evalue



			//if the nextNodePos is not a valid position for the current level, or the tile is not navigable, skip this node
			if (Level::isInLevel(currentLevel, nextNodePos.x, nextNodePos.y) == false
				|| currentLevel.tileMap.tiles[currentLevel.dim.x * nextNodePos.y + nextNodePos.x].logicType == LogicType::Wall)
				continue;
			//if the nextNodePos is not a valid position for the current level, or the tile is not navigable, skip this node



			//Search a node with nextNodePos in heap
			int found = minHeap.find(nextNodePos);
			//Search a node with nextNodePos in heap

			//If found a node with nextNodePos in the heap, update cost, estimation and father if found a shorter path
			if (found != -1)
			{
				PathNode nextNode;
				int index = found;

				nextNode.parent = &currentNode;
				nextNode.cost = currentNode.cost + 1;
				nextNode.estimation = (*estimation)(nextNodePos, start);
				nextNode.pos = nextNodePos;

				minHeap.updateKey(index, &nextNode);
			}
			//If found a node with nextNodePos in the heap, update cost, estimation and father if found a shorter path

			//If the node is not explored and is not in the heap, add to the heap
			if (graph.visited[nextNodePos.x + nextNodePos.y * world->currentLevel.dim.x] == false && found == -1)
			{
				PathNode* newNode = &graph.pathNodes[graph.nextNode];
				graph.nextNode++;
				newNode->pos = nextNodePos;
				newNode->cost = currentNode.cost + 1;
				newNode->estimation = (*estimation)(newNode->pos, start);
				newNode->parent = &currentNode;

				minHeap.insert(newNode);

				//Testing
				PathFindingSystem::exploredNodesCounter++;
				//Testing
			}
			//If the node is not explored and is not in the heap, add to the heap
		}
	}
	//Until the heap is empty loop

	graph.nextNode = 0;

	return nullptr;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PathFindingSystem
//-----------------------------------------------------------------------------------------------------------------------------------------