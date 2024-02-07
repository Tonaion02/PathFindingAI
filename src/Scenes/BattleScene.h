#pragma once

#include "ECS/Scene.h"





class BattleScene : public BaseScene
{
public:
	explicit BattleScene();

	void loadScene() override;
	void updateScene() override;
	void generateOutputScene() override;
	void processInputScene() override;

public:
	
protected:
};