#include "Item.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>

// Define static members
GLuint Item::healthPackTexture = 0;
GLuint Item::ammoPackTexture = 0;

Item::Item(const glm::vec3& position, ItemType type) :
    position(position), 
    type(type) 
{
	loadTextures();
    setupMesh();
}

void Item::loadTextures() 
{
    if (healthPackTexture == 0)
    {
        healthPackTexture = TextureLoader::LoadTexture("Textures/Items/HealthPack.png");
    }
    if (ammoPackTexture == 0) 
    {
        ammoPackTexture = TextureLoader::LoadTexture("Textures/Items/AmmoPack.png");
    }
}

void Item::setupMesh() 
{
    float vertices[] = 
    {
        // positions          // texture coords
         0.5f,  1.0f, 0.0f,   1.0f, 1.0f,   // Top Right
         0.5f,  0.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
        -0.5f,  0.0f, 0.0f,   0.0f, 0.0f,   // Bottom Left
        -0.5f,  1.0f, 0.0f,   0.0f, 1.0f    // Top Left
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

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Item::render(const glm::mat4& view, const glm::mat4& projection, Shader& shader) 
{
    glBindVertexArray(VAO);

    if (type == ItemType::HEALTH) 
    {
        glBindTexture(GL_TEXTURE_2D, healthPackTexture);
    }
    else if (type == ItemType::AMMO) 
    {
        glBindTexture(GL_TEXTURE_2D, ammoPackTexture);
    }


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // Billboarding: Make the quad face the camera
    glm::vec3 camRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
    glm::vec3 camUp = glm::vec3(view[0][1], view[1][1], view[2][1]);
    glm::vec3 lookAt = glm::normalize(position - glm::vec3(view[3]));

    glm::mat4 billboard = glm::mat4(1.0f);
    billboard[0] = glm::vec4(camRight, 0.0f);
    billboard[1] = glm::vec4(camUp, 0.0f);
    billboard[2] = glm::vec4(lookAt, 0.0f);

    model *= billboard;

	glm::vec3 size = glm::vec3(0.5f, 0.5f, 0.5f);
    model = glm::scale(model, size);

    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}