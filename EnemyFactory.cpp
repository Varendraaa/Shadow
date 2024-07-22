#include "EnemyFactory.h"
#include "Texture.h"
#include <unordered_map>
#include <iostream>

std::vector<AnimationFrame> loadFrames(const std::vector<std::string>& textures, const std::vector<int>& durations) 
{
	std::vector<AnimationFrame> frames;

	if (textures.size() != durations.size()) 
	{
		cerr << "Texture paths and frame durations must be the same length" << endl;
		return frames;
	}

	for (size_t i = 0; i < textures.size(); ++i) 
	{
		GLuint textureID = TextureLoader::LoadTexture(textures[i]);
		if (textureID == 0) 
		{
			cerr << "Failed to load texture: " << textures[i] << endl;
			continue;
		}
		frames.push_back({ textureID, durations[i] });
	}

	return frames;
}

Enemy createSoldier(const glm::vec3& position)
{
	static std::vector<AnimationFrame> walkFrames;
	static std::vector<AnimationFrame> attackFrames;
	static std::vector<AnimationFrame> deathFrames;
	
	if (walkFrames.empty() && attackFrames.empty() && deathFrames.empty())
	{
		vector<string> walkTextures =
		{
			"Sprites/Soldier/Walk1.png",
			"Sprites/Soldier/Walk2.png",
			"Sprites/Soldier/Walk3.png",
			"Sprites/Soldier/Walk4.png"
		};
		vector<int> walkDurations = { 100, 100, 100, 100 };

		vector<string> attackTextures =
		{
			"Sprites/Soldier/Attack1.png",
			"Sprites/Soldier/Attack2.png"
		};
		vector<int> attackDurations = { 100, 100 };

		vector<string> deathTextures =
		{
			"Sprites/Soldier/Death1.png",
			"Sprites/Soldier/Death2.png",
			"Sprites/Soldier/Death3.png",
			"Sprites/Soldier/Death4.png",
			"Sprites/Soldier/Death5.png"
		};
		vector<int> deathDurations = { 100, 100, 100, 100, 100 };

		walkFrames = loadFrames(walkTextures, walkDurations);
		attackFrames = loadFrames(attackTextures, attackDurations);
		deathFrames = loadFrames(deathTextures, deathDurations);
	}
	// Enemy Parameters
	int health = 60;
	float size = 1.5f;
	int damage = 5;

	return Enemy(position, health, size, damage, walkFrames, attackFrames, deathFrames);
}
