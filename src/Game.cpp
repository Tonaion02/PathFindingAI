#include "Game.h"

#include "SDL_Enviroment.h"

#include "Window/WindowHandler.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Game
//-----------------------------------------------------------------------------------------------------------------------------------------
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

}

void Game::init()
{
	WindowHandler::get();
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
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Game
//-----------------------------------------------------------------------------------------------------------------------------------------