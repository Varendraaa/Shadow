// Class is responsible of reading the TMX file and parsing its data.

#ifndef TMXPARSER_H
#define TMXPARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Object.hpp>

using namespace std;

struct TileLayer
{
	string name;
	int width;
	int height;
	vector<int> data;
};

struct Object
{
	string type;
	float x;
	float y;
	float width;
	float height;
};

class TiledMap
{
public:
	bool loadFromFile(const string& filePath);				// Loads data from the TMX file
	
	TileLayer getLayer(const string& layerName) const;		// Get a data from a specific layer
	vector<Object> getObjects(const string& objectType);	// Get a data from a specific object type

	// Entity Methods - remove to another class later
	glm::vec3 getPlayerSpawnPosition() const;
	vector<glm::vec3> getEnemySpawnPositions(const string& name) const;

private:
	vector <TileLayer> layers;	
	vector<Object> objects;

	tmx::Map map;

};


#endif // TMXPARSER_H

