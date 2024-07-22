#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

#include "Camera.h"
#include "TmxParser.h"
#include "LevelMesh.h"
#include "Weapon.h"
#include "SoundEngine.h"

using namespace std;

class Player : public Camera
{
public:
	
	Player(glm::vec3& spawnPoint);

	// Process Player Specific Input	
	void ProcessPlayerInput(bool* keys, float deltaTime, const TileLayer& wallLayer);
	
	// Check for Collision with walls
	bool checkCollision(glm::vec3& newPosition, const TileLayer& wallLayer);

	void RenderWeapon(Shader& shader);
	bool Shoot();

	void TakeDamage(int damage);
	int GetHealth() const { return health; }

	// Get methods
	glm::vec3 GetPosition() const;
	glm::vec3 GetFront() const;

	// PLayer Public Attributes
	int health;
	int ammo;	
	float shootCooldown;	
	float lastShotTime;

private:	
	// Player Specific Attributes
	glm::vec3 playerSize;		// Dimensions of the player's bounding box

	Weapon weapon;
	SoundEngine soundEngine;
};

#endif // !PLAYER_H

