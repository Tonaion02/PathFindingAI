#pragma once

#include <fstream>

#include "Systems/Exploring/PathFindingSystem.h"

#include "utils/Math/Vector2i.h"





class PerformanceTest
{
public:
	static void test();

private:
	static void allEuristic(const Vector2i& start, const Vector2i& end);
	static void singleTest(const Vector2i& start, const Vector2i& end, PathFindingSystem::MakeEstimation* euristic);

	static std::ofstream outputFile;
	static unsigned int counter;
	static std::string euristicUsed;
	static const std::string filePath;

	static const std::string euclidean;
	static const std::string manhattan;
	static const std::string cebyshev;
};