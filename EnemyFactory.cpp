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
	int health = 20;
	float size = 1.5f;
	int damage = 5;

	return Enemy(position, health, size, damage, walkFrames, attackFrames, deathFrames);
}

//Enemy createZombie(const glm::vec3& position)
//{
//	
//	static std::vector<AnimationFrame> walkFrames;
//	static std::vector<AnimationFrame> attackFrames;
//	static std::vector<AnimationFrame> deathFrames;
//	
//
//	if (walkFrames.empty() && attackFrames.empty() && deathFrames.empty())
//	{
//		vector<string> walkTextures =
//		{
//			"Sprites/Zombie/walk1.png",
//			"Sprites/Zombie/walk2.png",
//			"Sprites/Zombie/walk3.png",
//			"Sprites/Zombie/walk4.png",
//			"Sprites/Zombie/walk5.png",
//			"Sprites/Zombie/walk6.png",
//			"Sprites/Zombie/walk7.png",
//			"Sprites/Zombie/walk8.png",
//			"Sprites/Zombie/walk9.png",
//			"Sprites/Zombie/walk10.png",
//			"Sprites/Zombie/walk11.png",
//			"Sprites/Zombie/walk12.png",
//			"Sprites/Zombie/walk13.png",
//			"Sprites/Zombie/walk14.png",
//			"Sprites/Zombie/walk15.png",
//			"Sprites/Zombie/walk16.png",
//			"Sprites/Zombie/walk17.png"
//
//		};
//		vector<int> walkDurations = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
//
//		vector<string> attackTextures =
//		{
//			"Sprites/Zombie/attack1.png",
//			"Sprites/Zombie/attack2.png",
//			"Sprites/Zombie/attack3.png",
//			"Sprites/Zombie/attack4.png",
//			"Sprites/Zombie/attack5.png",
//			"Sprites/Zombie/attack6.png",
//			"Sprites/Zombie/attack7.png",
//			"Sprites/Zombie/attack8.png",
//			"Sprites/Zombie/attack9.png",
//			"Sprites/Zombie/attack10.png",
//			"Sprites/Zombie/attack11.png",
//			"Sprites/Zombie/attack12.png",
//			"Sprites/Zombie/attack13.png",
//			"Sprites/Zombie/attack14.png",
//			"Sprites/Zombie/attack15.png",
//			"Sprites/Zombie/attack16.png",
//			"Sprites/Zombie/attack17.png",
//			"Sprites/Zombie/attack18.png",
//			"Sprites/Zombie/attack19.png"
//
//		};
//		vector<int> attackDurations = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
//
//		vector<string> deathTextures =
//		{
//			"Sprites/Zombie/dying1.png",
//			"Sprites/Zombie/dying2.png",
//			"Sprites/Zombie/dying3.png",
//			"Sprites/Zombie/dying4.png",
//			"Sprites/Zombie/dying5.png",
//			"Sprites/Zombie/dying6.png",
//			"Sprites/Zombie/dying7.png",
//			"Sprites/Zombie/dying8.png",
//			"Sprites/Zombie/dying9.png",
//			"Sprites/Zombie/dying10.png",
//			"Sprites/Zombie/dying11.png",
//			"Sprites/Zombie/dying12.png",
//			"Sprites/Zombie/dying13.png",
//			"Sprites/Zombie/dying14.png",
//			"Sprites/Zombie/dying15.png",
//			"Sprites/Zombie/dying16.png",
//			"Sprites/Zombie/dying17.png",
//			"Sprites/Zombie/dying18.png",
//			"Sprites/Zombie/dying19.png",
//			"Sprites/Zombie/dying20.png",
//			"Sprites/Zombie/dying21.png",
//			"Sprites/Zombie/dying22.png",
//			"Sprites/Zombie/dying23.png"
//		};
//		vector<int> deathDurations = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
//
//		walkFrames = loadFrames(walkTextures, walkDurations);
//		attackFrames = loadFrames(attackTextures, attackDurations);
//		deathFrames = loadFrames(deathTextures, deathDurations);
//	}
//	// Enemy Parameters
//	int health = 100;
//	float size = 1.3f;
//	int damage = 10;
//	
//	
//	return Enemy(position, health, size, damage, walkFrames, attackFrames, deathFrames);
//}
