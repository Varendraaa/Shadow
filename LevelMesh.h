#pragma once
#include <vector>
#include <iostream>
#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "TmxParser.h"
#include "Vertex.h"

using namespace std;

class LevelMesh 
{
public:
    LevelMesh(const string& levelPath);
    ~LevelMesh();

    void loadTextures();
    void generateMesh(const string& levelPath);
    void draw();
    void cleanup();

private:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<unsigned int> floorIndices;
    vector<unsigned int> wallIndices;
    vector<unsigned int> ceilingIndices;

	map<int, unsigned int> textureIDs;
    GLuint VAO, VBO, EBO;
    //GLuint floorTexture, wallTexture, ceilingTexture;

    void addFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 normal, vector<unsigned int>& targetIndices, unsigned int textureID);
    void setupMesh();

    bool isBlocked(int x, int z, const TileLayer& layout);


};