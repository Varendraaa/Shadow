#include "Player.h"


Player::Player(glm::vec3& spawnPoint) :
	Camera(spawnPoint + glm::vec3(0.0f, 0.8f, 0.0f)),
	playerSize(0.5f, 1.0f, 0.5f),
	health(100),
	ammo(100),
	shootCooldown(0.5f),
	lastShotTime(0.0f)
	
{	
}

// Process Player Specific Input
void Player::ProcessPlayerInput(bool* keys, float deltaTime, const TileLayer& wallLayer)
{
	glm::vec3 oldPosition = Position;
	ProcessKeyboard(keys, deltaTime);

	// Collision Check Logic	
	glm::vec3 newPosition = Position;			

	if (checkCollision(newPosition, wallLayer))
	{
		// If collision detected, resolve by moving along each axis independently
		newPosition = oldPosition;		
		newPosition.x = Position.x;
		if (checkCollision(newPosition, wallLayer))
		{
			newPosition.x = oldPosition.x;		// Revert x position if collision detected	
		}

		newPosition.z = Position.z;
		if (checkCollision(newPosition, wallLayer))
		{
			newPosition.z = oldPosition.z;		// Revert z position if collision detected
		}
	}	
	
	Position = newPosition;						// Update the player's position

	if (keys[GLFW_MOUSE_BUTTON_1] && glfwGetTime() - lastShotTime >= shootCooldown) 
	{		
		//weapon.Shoot();
		Shoot();
		lastShotTime = glfwGetTime();
	}
	weapon.Update(deltaTime);
}


// Check for Collision
bool Player::checkCollision(glm::vec3& newPosition, const TileLayer& wallLayer)
{
	// Calculate the bounding box corners of the player;
	glm::vec3 playerMin = newPosition - playerSize / 2.0f;
	glm::vec3 playerMax = newPosition + playerSize / 2.0f;

	// Iterate through the tiles in the wall layer and check for overlap
	for (int z = 0; z < wallLayer.height; ++z)
	{
		for (int x = 0; x < wallLayer.width; ++x)
		{
			if (wallLayer.data[z * wallLayer.width + x] != 0)		// There is a wall tile here
			{
				glm::vec3 wallMin(x, 0.0f, z);
				glm::vec3 wallMax(x + 1.0f, 2.0f, z + 1.0f);		// Change y value to match wall height

				if ((playerMin.x < wallMax.x && playerMax.x > wallMin.x) &&
					(playerMin.y < wallMax.y && playerMax.y > wallMin.y) &&	
					(playerMin.z < wallMax.z && playerMax.z > wallMin.z))
				{
					return true;		// Collision detected
				}
			}
		}
	}
	return false;	// No collision detected	
}

void Player::RenderWeapon(Shader& shader) 
{
	weapon.Render(glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f), shader);
}

bool Player::Shoot()
{
	if (ammo > 0)
	{
		weapon.Shoot();
		ammo--;

		// Add sound effect here  - instance of the sound engine
		irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
		if (soundEngine) 
		{
			soundEngine->setSoundVolume(0.2f);
			soundEngine->play2D("Music/w_rifle.ogg");
		}
		return true;
	}
	return false;
}

void Player::TakeDamage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		health = 0;
		cout << "Player is dead!" << endl;
	}
}

// Get methods
glm::vec3 Player::GetPosition() const 
{
	return Position;
}

glm::vec3 Player::GetFront() const 
{
	return Front;
}