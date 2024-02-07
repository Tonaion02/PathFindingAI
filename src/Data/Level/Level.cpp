#include "Level.h"

#include "Game.h"

#include "Utils/StringAndFile/XMLvariab.h"
#include "Utils/StringAndFile/MyString.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Level
//-----------------------------------------------------------------------------------------------------------------------------------------
Level Level::loadLevelFromFile(const std::string & filePath)
{
	Level level = Level();

	std::vector<std::string> lines = getLines(filePath);
	std::vector<XMLvariab> xmlVariables = getXMLvariables(lines);

	XMLvariab principle = xmlVariables[0];

	Vector2i levelDim;
	levelDim.x = stoi(principle.getValue("width"));
	levelDim.y = stoi(principle.getValue("height"));
	level.dim = levelDim;

	unsigned int tileDim = stoi(principle.getValue("tilewidth"));

	xmlVariables = getXMLvariables(principle.rawData);











	return level;
}



void Level::initMap()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Level
//-----------------------------------------------------------------------------------------------------------------------------------------