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

	TileSetHandler* getTileSetHandler() const;
	Level* getCurrentLevel() const;

private:
	void init();
	void loadData();
	void update();
	void draw();
	void processInput();

	Game() {}

public:
	static const std::string baseDataPath;

	static const std::string levelFilePath;

private:
	struct Level* currentLevel;
	class TileSetHandler* tileSetHandler;
};