#include "Weapon.h"

Weapon::Weapon() :
	currentFrame(0), 
	timeSinceLastFrame(0.0f), 
	isFiring(false), 
	bobbingTime(0.0f), 
	bobbingAmplitude(3.0f), 
	bobbingFrequency(2.0f)
{
	setupMesh();
	loadTextures();
}

void Weapon::loadTextures()
{
	vector<string> texturePath =
	{
		"Sprites/Gun_1.png",
		"Sprites/Gun_2.png",
		"Sprites/Gun_3.png",
		"Sprites/Gun_4.png",
		"Sprites/Gun_5.png",
		"Sprites/Gun_6.png"
	};
	vector<int> frameDuration = { 100, 50, 100, 40, 200, 200 };	 // Duration in milliseconds for each frame


	if (texturePath.size() != frameDuration.size()) 
	{
		cerr << "Texture paths and frame durations must be the same length" << endl;
		return;
	}

	for (size_t i = 0; i < texturePath.size(); ++i) {
		AnimationFrame frame;
		frame.textureID = TextureLoader::LoadTexture(texturePath[i]);
		frame.duration = frameDuration[i];
		frames.push_back(frame);
	}
}


void Weapon::setupMesh()
{
	float vertices[] =
	{
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Weapon::Update(float deltaTime)
{
	if (isFiring)
	{
		timeSinceLastFrame += deltaTime * 1000.0f; // Convert deltaTime to milliseconds
		if (timeSinceLastFrame >= frames[currentFrame].duration)
		{
			timeSinceLastFrame = 0.0f;
			nextFrame();
		}
	}
	bobbingTime += deltaTime * bobbingFrequency;
}


void Weapon::Render(const glm::mat4& projection, Shader& shader)
{
	glDisable(GL_DEPTH_TEST); // Disable depth testing

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, frames[currentFrame].textureID);

	// Create orthographic projection for screen space
	glm::mat4 orthoProjection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f); // Adjust based on your window size

	glm::mat4 model = glm::mat4(1.0f);
	float bobbingOffset = sin(bobbingTime) * bobbingAmplitude;
	//Translate, scale and rotate the weapon to desired position on screen
	model = glm::translate(model, glm::vec3(900.0f, 80.0f + bobbingOffset, 0.0f)); // Center of the screen
	model = glm::scale(model, glm::vec3(300.0f, 200.0f, 1.0f)); // Adjust size

	
	shader.Use();
	shader.SetMat4("model", model);
	shader.SetMat4("view", glm::mat4(1.0f)); // Identity matrix for view
	shader.SetMat4("projection", orthoProjection);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST); // Re-enable depth testing
}


void Weapon::Shoot()
{
	isFiring = true;
	currentFrame = 0;
	timeSinceLastFrame = 0.0f;
}

void Weapon::nextFrame() 
{
	currentFrame++;
	if (currentFrame >= frames.size()) {
		currentFrame = 0;
		isFiring = false;
	}
}