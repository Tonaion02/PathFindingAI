#pragma once





class Game
{
public:
	void run();

private:
	void init();
	void loadData();
	void update();
	void draw();
	void processInput();
};