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



struct ComparePathNode
{
	bool operator()(PathNode* l, PathNode* r)
	{
		return l->cost + l->estimation > r->cost + r->estimation;
	}
};


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

	std::vector<PathNode*> heap;
};



void PathFindingSystem::initGraph()
{
	World* world = Game::get()->getWorld();

	world->graph.nextNode = 0;
	world->graph.pathNodes.resize(MAX_H_MAP * MAX_W_MAP);
	world->graph.visited.resize(MAX_H_MAP * MAX_W_MAP);
}



PathNode* PathFindingSystem::findPath(const Vector2i& start, const Vector2i& end, MakeEstimation* estimation)
{
	World* world = Game::get()->getWorld();
	Graph& graph = world->graph;
	Level& currentLevel = world->currentLevel;


	//Reset graph struct
	graph.nextNode = 0;
	std::fill(world->graph.visited.begin(), world->graph.visited.end(), false);
	//Reset graph struct


	MinHeap minHeap(MAX_H_MAP * MAX_W_MAP);
	PathNode& pathNode = graph.pathNodes[graph.nextNode];
	
	pathNode.cost = 0.0f;
	pathNode.estimation = (*estimation)(start, end);
	pathNode.parent = nullptr;
	pathNode.pos = start;
	minHeap.insert(&pathNode);
	
	//graph.nextNode++;
	//PathNode& testNode = graph.pathNodes[graph.nextNode];
	//testNode.cost = 0.0f;
	//testNode.estimation = 1.0f;
	//testNode.pos = start;
	//testNode.pos.x += 1;
	//minHeap.insert(&testNode);

	//graph.nextNode++;
	//PathNode& testNode2 = graph.pathNodes[graph.nextNode];
	//testNode2.cost = 0.0f;
	//testNode2.estimation = 2.0f;
	//testNode2.pos = start;
	//testNode2.pos.y += 1;
	//minHeap.insert(&testNode2);

	graph.nextNode++;
	

	while (!minHeap.empty())
	{
		
		//int index = minHeap.find({ 1, 10 });

		//PathNode* currentNode = minHeap.top();
		//minHeap.pop();

		//PathNode* testNode = minHeap.top();
		//minHeap.pop();

		//PathNode* testNode2p = minHeap.top();
		//minHeap.pop();

		//

		//SDL_Log(std::to_string(*currentNode > *testNode).c_str());

		PathNode& currentNode = *minHeap.top();
		minHeap.pop();

		if (currentNode.pos.x == end.x && currentNode.pos.y == end.y)
		{
			return &currentNode;
		}

		graph.visited[currentNode.pos.x + currentNode.pos.y * world->currentLevel.dim.x] = true;



		Vector2i nextNodePos;
		for (int i = 0; i < dx.size(); i++)
		{
			nextNodePos.x = currentNode.pos.x + dx[i];
			nextNodePos.y = currentNode.pos.y + dy[i];



			if (Level::isInLevel(currentLevel, nextNodePos.x, nextNodePos.y) == false
				|| currentLevel.tileMap.tiles[currentLevel.dim.x * nextNodePos.y + nextNodePos.x].logicType == LogicType::Wall)
				continue;




			//Aggiornamento
			if (minHeap.find(nextNodePos) != -1)
			{
				PathNode nextNode;
				nextNode.pos = nextNodePos;

				int index = minHeap.find(nextNodePos);
				if (index != -1)
				{
					nextNode.parent = &currentNode;
					nextNode.cost = currentNode.cost + 1;
					nextNode.estimation = (*estimation)(nextNodePos, end);

					minHeap.updateKey(index, &nextNode);
				}
			}

			//Prima volta che lo vedo
			int found = minHeap.find(nextNodePos);

			if (graph.visited[nextNodePos.x + nextNodePos.y * world->currentLevel.dim.x] == false && found == -1)
			{
				PathNode* newNode = &graph.pathNodes[graph.nextNode];
				graph.nextNode++;
				newNode->pos = nextNodePos;
				newNode->cost = currentNode.cost + 1;
				newNode->estimation = (*estimation)(newNode->pos, end);
				newNode->parent = &currentNode;

				minHeap.insert(newNode);
			}
		}
	}

	graph.nextNode = 0;

	return nullptr;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PathFindingSystem
//-----------------------------------------------------------------------------------------------------------------------------------------