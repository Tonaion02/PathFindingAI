#pragma once

#include <fstream>

#include "Systems/Exploring/PathFindingSystem.h"

#include "utils/Math/Vector2i.h"




class PerformanceTest
{
public:
	static void test();

private:
	static void singleTest(const Vector2i& start, const Vector2i& end, PathFindingSystem::MakeEstimation* euristic);

	static std::ofstream outputFile;
	static unsigned int counter;
	static const std::string filePath;
};