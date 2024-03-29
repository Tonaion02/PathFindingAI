#pragma once

#include "ECS/Entity.h"
#include "PathFindingSystem.h"





class EnemySystem
{
	friend class RenderDebugFieldOfView;
public:
	static void init();
	static void aiBaseEnemy();
protected:
	static void reversePath(Entity e);

	static unsigned int computeTriangleBase(int viewDistance);

public:
	static const unsigned int angleView;

	static PathFindingSystem::MakeEstimation* euristic;
};