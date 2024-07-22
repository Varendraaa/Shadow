#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Shader.h"
#include "TmxParser.h"

using namespace std;

enum ItemType
{
	HEALTH,
	AMMO
};

class Item
{
public:
	Item(const glm::vec3& position, ItemType type);
	void render(const glm::mat4& view, const glm::mat4& projection, Shader& shader);

	glm::vec3 getPosition() const { return position; }
	ItemType getType() const { return type; }

	// Static methods to load textures
	static void loadTextures();

private:
	glm::vec3 position;
	ItemType type;
	
	static GLuint healthPackTexture;
	static GLuint ammoPackTexture;

	GLuint VAO, VBO, EBO;
	void setupMesh();
};
