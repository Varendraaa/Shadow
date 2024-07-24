#include "TmxParser.h"

bool TiledMap::loadFromFile(const string& filePath)
{
	if (!map.load(filePath))
	{
		cerr << "Failed to load TMX file! " << endl;
		return false;
	}
	cout << "Successfully loaded TMX file! " << filePath << endl;


	// Parse File Data
	for (const auto& layer : map.getLayers())
	{
		if (layer->getType() == tmx::Layer::Type::Tile)
		{
			const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
			TileLayer layerData;
			layerData.name = tileLayer.getName();
			layerData.width = tileLayer.getSize().x;
			layerData.height = tileLayer.getSize().y;

			const auto& tiles = tileLayer.getTiles();
			for (const auto& tile : tiles)
			{
				layerData.data.push_back(tile.ID);
			}

			layers.push_back(layerData);
			cout << "Loaded Tile Layer: " << layerData.name << endl;
		}
		else if (layer->getType() == tmx::Layer::Type::Object)
		{
			const auto& objectGroup = layer->getLayerAs<tmx::ObjectGroup>();
			for (const auto& object : objectGroup.getObjects())
			{
				Object objData;						// Create a new Object struct
				objData.type = object.getName();
				objData.x = object.getPosition().x;
				objData.y = object.getPosition().y;
				objData.width = object.getAABB().width;
				objData.height = object.getAABB().height;

				objects.push_back(objData);			// Add data from objData to objects vector

				//// Debugging output to verify parsed objects
				//cout << "Parsed Object - Type: " << objData.type
				//	<< ", X: " << objData.x
				//	<< ", Y: " << objData.y
				//	<< ", Width: " << objData.width
				//	<< ", Height: " << objData.height << endl;
			}
		}
	}
	return true;
}

TileLayer TiledMap::getLayer(const string& layerName) const 
{
	for (const auto& layer : layers)
	{
		if (layer.name == layerName)
			return layer;
	}

	//Return Empty layer if not found
	cerr << "Layer " << layerName << "not found. " << endl;
	return TileLayer();
}


vector<Object> TiledMap::getObjects(const string& objectType)
{
	vector<Object> result;
	for (const auto& object : objects) 
	{
		if (object.type == objectType) 
		{
			result.push_back(object);
		}
	}
	return result;
}

// Entity Spawn
glm::vec3 TiledMap::getPlayerSpawnPosition() const
{
	for (const auto& object : objects)
	{
		if (object.type == "Player")
		{
			cout << "Player Spawn Position Found! : " << object.x << ", " << object.y << endl;
			return glm::vec3(object.x / 16, 0.0f, object.y / 16);		// Divide by tile size (16) to get position in world space
		}
	}

	// Default spawn position if none found
	cerr << "Player Spawn Position not found! Using default spawn position." << endl;
	return glm::vec3(4.0f, 0.0f, 4.0f);
}

vector<glm::vec3> TiledMap::getEnemySpawnPositions(const string& name) const
{
	vector<glm::vec3> enemySpawns;
	for (const auto& object : objects)
	{
		if (object.type == name)
		{
			cout << "Enemy Spawn Position Found! : " << object.x << ", " << object.y << endl;
			enemySpawns.push_back(glm::vec3(object.x /16, 0.0f, object.y /16));
		}
	}
	return enemySpawns;
}

