#include "Level.h"

#include "Game.h"

#include "Utils/StringAndFile/XMLvariab.h"
#include "Utils/StringAndFile/MyString.h"





//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Level
//-----------------------------------------------------------------------------------------------------------------------------------------
Level* Level::loadLevelFromFile(const std::string & filePath)
{
	Level* level_ = new Level();
	Level& level = *level_;

	std::vector<std::string> lines = getLines(filePath);
	std::vector<XMLvariab> xmlVariables = getXMLvariables(lines);

	XMLvariab principle = xmlVariables[0];

	Vector2i levelDim;
	levelDim.x = stoi(principle.getValue("width"));
	levelDim.y = stoi(principle.getValue("height"));
	level.dim = levelDim;

	unsigned int tileDim = stoi(principle.getValue("tilewidth"));

	xmlVariables = getXMLvariables(principle.rawData);



	Vector2i posG = { 0, 0 };
	int zG = 0;

	Vector2i posT = { 0, 0 };
	int zT = 0;

	int firstGId;

	int indexGroup = -1;


	while (!xmlVariables.empty())
	{
		XMLvariab var = xmlVariables.front();
		xmlVariables.erase(xmlVariables.begin());



		//Analyze the xml variables
		if (var.name == "tileset")
		{
			//std::string path = var.getValue("source");
			//path = baseDataPath + path.substr(0, path.find(".")) + ".png";

			//if (path != "data/tekTileSet.png")
			//	level.tileSet = world->mTileSetHandler.getTileSet(path);
			//else
			//	firstGId = std::stoi(var.getValue("firstgid"));
		}
		else if (var.name == "group")
		{
			indexGroup++;

			level.graphicTileLayer.deepOfLayer.push_back(0);

			std::vector<XMLvariab> copy = getXMLvariables(var.rawData);

			while (!copy.empty())
			{
				xmlVariables.emplace(xmlVariables.begin(), copy.back());

				copy.pop_back();
			}
		}
		else if (var.name == "layer")
		{
			//Update the counter of layer for this group
			level.graphicTileLayer.deepOfLayer[indexGroup]++;
			//Update the counter of layer for this group

			XMLvariab var2 = getXMLvariables(var.rawData)[0];

			for (unsigned int j = 0; j < var2.rawData.size(); j++)
			{
				std::string line = var2.rawData[j];
				std::vector<std::string> splittedLine = split(line, ",");

				for (auto stringValue : splittedLine)
				{
					level.graphicTileLayer.gTiles[zG * levelDim.x * levelDim.y + posG.y * levelDim.x + posG.x] = stoi(stringValue) - 1;

					posG.x++;
					if (posG.x == levelDim.x)
					{
						posG.y++;
						posG.x = 0;
					}
				}
			}

			zG++;
			posG.x = 0;
			posG.y = 0;
		}
		else if (var.name == "objectgroup")
		{
			//Distinguish from name beetween entity layer and tecnical layer


			//Set all the tiles to Ground
			level.tileMap.tiles.resize(levelDim.x * levelDim.y * (zT + 1));

			for (posT.y = 0; posT.y < levelDim.y; posT.y++)
			{
				for (posT.x = 0; posT.x < levelDim.x; posT.x++)
				{
					level.tileMap.tiles[zT * levelDim.y * levelDim.x + posT.y * levelDim.x + posT.x].logicType = static_cast<short int>(LogicType::Ground);
				}
			}
			//Set all the tiles to Ground



			////Set all the Unique tiles to nullptr
			//level.tileMap.uniqueTiles.resize(levelDim.x * levelDim.y * (zT + 1));


			//for (posT.y = 0; posT.y < levelDim.y; posT.y++)
			//{
			//	for (posT.x = 0; posT.x < levelDim.x; posT.x++)
			//	{
			//		level.tileMap.uniqueTiles[zT * levelDim.y * levelDim.x + posT.y * levelDim.x + posT.x] = nullptr;
			//	}
			//}
			////Set all the Unique tiles to nullptr



			//Set all the tiles to unOccupied
			level.tileMap.mappedEntities.resize(levelDim.x * levelDim.y * (zT + 1));

			for (posT.y = 0; posT.y < levelDim.y; posT.y++)
			{
				for (posT.x = 0; posT.x < levelDim.x; posT.x++)
				{
					level.tileMap.mappedEntities[zT * levelDim.y * levelDim.x + posT.y * levelDim.x + posT.x] = static_cast<int>(EntityOccupier::NoneEntityOccupier);
				}
			}
			//Set all the tiles to unOccupied



			////Add particular object to this layer
			//if (var.withRawData)
			//{
			//	std::vector<XMLvariab> variables = getXMLvariables(var.rawData);
			//	for (auto iter : variables)
			//	{
			//		int logicType = 0;
			//		//Get the pos of the Tile
			//		Vector2i pos = { std::stoi(iter.getValue("x")), std::stoi(iter.getValue("y")) - world->currentLevel.tileSet->tileDim.y };
			//		pos = pos / world->currentLevel.tileSet->tileDim;
			//		//Get the pos of the Tile

			//		//Case is a standard Tile
			//		if (iter.getValue("gid") != "nothing")
			//		{
			//			logicType = std::stoi(iter.getValue("gid")) - firstGId;
			//		}
			//		//Case is a standard Tile

			//		//Case is a Unique Tile
			//		else
			//		{
			//			//Create the Unique Tile
			//			XMLvariab temp = *exploringScene->templatesUniqueTile[iter.getValue("template")];
			//			logicType = std::stoi(temp.getValue("gid")) - 1;

			//			//In case some properties are modified
			//			if (iter.withRawData)
			//			{
			//				//Delete the first line and last line("properties")
			//				iter.rawData.erase(iter.rawData.begin());
			//				iter.rawData.pop_back();
			//				//Delete the first line and last line("properties")

			//				//Iterate about properties that is changed
			//				std::vector<XMLvariab> propertiesChanged = getXMLvariables(iter.rawData);
			//				for (XMLvariab& prop : propertiesChanged)
			//					temp.values[prop.getValue("name")] = prop.getValue("value");
			//				//Iterate about properties that is changed
			//			}
			//			//In case some properties are modified

			//			//Add Unique Tile to the collection of the level of UniqueTile
			//			level.tileMap.uniqueTiles[zT * levelDim.y * levelDim.x + pos.y * levelDim.x + pos.x] = TileSystem::createUniqueTile(&temp);
			//			//Add Unique Tile to the collection of the level of UniqueTile

			//			//Create the Unique Tile
			//		}
			//		//Case is a Unique Tile

			//		//Set the logicType of Tile
			//		level.tileMap.tiles[zT * levelDim.y * levelDim.x + pos.y * levelDim.x + pos.x].logicType = static_cast<int>(logicType);
			//		//Set the logicType of Tile
			//	}
			//}
			////Add particular object to this layer



			//Add a group of Entity
			//level.groupsEntities.push_back(GroupEntity<>());
			//Add a group of Entity



			zT++;
		}
		//Analyze the xml variables
	}



	level.maxZ = zT;







	return level_;
}



void Level::initMap()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Class Level
//-----------------------------------------------------------------------------------------------------------------------------------------