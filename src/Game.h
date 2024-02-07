#pragma once

#include "Data/Level/Level.h"





class Game
{
public:
	static Game& get()
	{
		static Game instance = Game();
		return instance;
	}

	void run();

private:
	void init();
	void loadData();
	void update();
	void draw();
	void processInput();

	Game() {}

public:
	const std::string baseDataPath = "data/";

	const std::string levelFilePath = "data/level0.tmx";

private:
	struct Level* currentLevel;
};