#include "Game.h"

#include "SDL_Enviroment.h"

#include "Window/WindowHandler.h"

#include "TextureHandler/TextureHandler.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Game
//-----------------------------------------------------------------------------------------------------------------------------------------
const std::string Game::baseDataPath = "data/";

const std::string Game::levelFilePath = "data/stressTestLevel.tmx";





void Game::update()
{

}



void Game::draw()
{
	//Clean screen
	SDL_SetRenderDrawColor(WindowHandler::get().getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(WindowHandler::get().getRenderer());
	//Clean screen



	//Render elements
	SDL_RenderPresent(WindowHandler::get().getRenderer());
	//Render elements
}



void Game::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{

	}
}



void Game::loadData()
{
	int tileDim = 32;

	tileSetHandler = new TileSetHandler();

	tileSetHandler->loadTileSet("data/buch-outdoor.png", tileDim);

	currentLevel = Level::loadLevelFromFile(Game::levelFilePath);
	SDL_Log("");
}



void Game::init()
{
	WindowHandler::get();

	TextureHandler::get();
}



void Game::run()
{
	init();
	loadData();

	while (true)
	{
		processInput();
		update();
		draw();
	}
}




TileSetHandler * Game::getTileSetHandler() const
{
	return tileSetHandler;
}

Level * Game::getCurrentLevel() const
{
	return currentLevel;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Game
//-----------------------------------------------------------------------------------------------------------------------------------------