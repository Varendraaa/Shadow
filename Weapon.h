#ifndef WEAPON_H
#define WEAPON_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "Texture.h"
#include "Shader.h"	

using namespace std;

class Weapon
{
public:
	Weapon();

	void loadTextures();

	void Render(const glm::mat4& projection, Shader& shader);
	void Update(float deltaTime);
	void Shoot();

private:
	struct AnimationFrame
	{
		GLuint textureID;
		int duration;
	};

	vector<AnimationFrame> frames;	

	int currentFrame;
	float timeSinceLastFrame;
	bool isFiring;

	float bobbingTime;
	float bobbingAmplitude;
	float bobbingFrequency;


	GLuint VAO, VBO, EBO;

	void setupMesh();
	void nextFrame();
};

#endif // !WEAPON_H

