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

const std::string PerformanceTest::euclidean = "euclidean";
const std::string PerformanceTest::manhattan = "manhattan";
const std::string PerformanceTest::cebyshev = "cebyshev";
std::string PerformanceTest::euristicUsed = "euclidean";





void PerformanceTest::test()
{
	World* world = Game::get()->getWorld();


	PerformanceTest::counter = 1;

	outputFile = std::ofstream(PerformanceTest::filePath);


	PerformanceTest::allEuristic({ 7, 13 }, { 18, 0 });
	PerformanceTest::allEuristic({ 7, 13 }, { 18, 0 });
	PerformanceTest::allEuristic({ 33, 1 }, { 75, 36 });
	PerformanceTest::allEuristic({ 156, 52 }, {20, 71});
	PerformanceTest::allEuristic({ 79, 36 }, { 158, 1 });
	PerformanceTest::allEuristic({ 3, 130 }, { 61, 125 });
	PerformanceTest::allEuristic({ 29, 103 }, { 33, 155 });


	outputFile.close();
}



void PerformanceTest::allEuristic(const Vector2i& start, const Vector2i& end)
{
	World* world = Game::get()->getWorld();

	PerformanceTest::euristicUsed = PerformanceTest::manhattan;
	singleTest({ start }, { end }, world->manhattan);
	PerformanceTest::euristicUsed = PerformanceTest::euclidean;
	singleTest({ start }, { end }, world->euclidean);
	PerformanceTest::euristicUsed = PerformanceTest::cebyshev;
	singleTest({ start }, { end }, world->chebyshev);
	PerformanceTest::counter++;
}



void PerformanceTest::singleTest(const Vector2i& start, const Vector2i& end, PathFindingSystem::MakeEstimation* euristic)
{
	auto t1 = std::chrono::high_resolution_clock::now();
	PathNode* pathNode = PathFindingSystem::findPath(start, end, euristic);
	auto t2 = std::chrono::high_resolution_clock::now();

	//Getting number of milliseconds as a double
	auto ns = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	//Getting number of milliseconds as a double

	std::string testOutput = "Test " + std::to_string(counter) + ": " + "Euristic: " + PerformanceTest::euristicUsed + " Elapsed time: " + std::to_string(ns.count()) + "micros" + " Nodes explored: " + std::to_string(PathFindingSystem::getExploredNodesCounter()) +"\n";
	
	outputFile << testOutput.c_str();

	outputFile.flush();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class PerformanceTest
//-----------------------------------------------------------------------------------------------------------------------------------------