#include "PerformanceTest.h"

#include "Game.h"
#include "World.h"

#include <string>

#include <chrono>
#include <thread>





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PerformanceTest
//-----------------------------------------------------------------------------------------------------------------------------------------
std::ofstream PerformanceTest::outputFile;

const std::string PerformanceTest::filePath = "data/Test/Result/PerformanceResult.txt";

unsigned int PerformanceTest::counter = 0;





void PerformanceTest::test()
{
	World* world = Game::get()->getWorld();


	counter = 0;

	outputFile = std::ofstream(PerformanceTest::filePath);

	singleTest({10,10}, {159, 159}, world->chebyshev);




	outputFile.close();
}

void PerformanceTest::singleTest(const Vector2i& start, const Vector2i& end, PathFindingSystem::MakeEstimation* euristic)
{
	counter++;


	auto t1 = std::chrono::high_resolution_clock::now();
	PathNode* pathNode = PathFindingSystem::findPath(start, end, euristic);
	auto t2 = std::chrono::high_resolution_clock::now();

	//Getting number of milliseconds as a double
	auto ns = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	//Getting number of milliseconds as a double

	std::string testOutput = "Test " + std::to_string(counter) + ": " + std::to_string(ns.count()) + "micros" + " Nodes explored: " + std::to_string(PathFindingSystem::getExploredNodesCounter()) +"\n";
	
	outputFile << testOutput.c_str();

	outputFile.flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PerformanceTest
//-----------------------------------------------------------------------------------------------------------------------------------------