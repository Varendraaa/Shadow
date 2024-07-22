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



// //Class to represent and Manage the Level Mesh
//class LevelMesh
//{
//public:
//	LevelMesh();            // Constructor
//	~LevelMesh();           // Destructor
//        
//	void loadTextures();    // Load textures for the level -> calls TextureLoader code
//	void generateMesh();    // Generate the mesh from the layouts
//	void draw();            // Render the Mesh
//
//private:
//
//	vector<Vertex> vertices;        // List of vertices
//	vector<GLuint> indices;         // List of indices for all elements
//
//    vector<unsigned int> floorIndices;      // Indices for the floor
//    vector<unsigned int> wallIndices;       // Indices for the walls
//    vector<unsigned int> ceilingIndices;    // Indices for the ceilings
//
//    // OpenGL Array, Buffer and Texture Objects
//	GLuint VAO, VBO, EBO;                   
//	GLuint floorTexture, wallTexture, ceilingTexture;
//
//    
//	void addCube(float x, float y, float z, vector<unsigned int>& targetIndices);   // Add a cube at a given position
//    void setupMesh();                   // Setup mesh for the given indices

class LevelMesh 
{
public:
    LevelMesh();
    ~LevelMesh();

    void loadTextures();
    void generateMesh();
    void draw();
    void cleanup();

private:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<unsigned int> floorIndices;
    vector<unsigned int> wallIndices;
    vector<unsigned int> ceilingIndices;

    GLuint VAO, VBO, EBO;
    GLuint floorTexture, wallTexture, ceilingTexture;

    void addFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 normal, vector<unsigned int>& targetIndices);
    void setupMesh();
    //bool isBlocked(int x, int z, int layout[10][10]);
    bool isBlocked(int x, int z, const TileLayer& layout);




    //// Constants for the level Size
    //const int levelWidth = 10;
    //const int levelHeight = 10;

    //// Static cube vertex data (avoiding redundancy in addCube)
    //static const Vertex cubeVertices[24];
    //static const unsigned int cubeIndices[36];

    //// Hard coded layouts
    //int floorLayout[10][10] = {
    //    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    //};

    //int wallLayout[10][10] = {
    //    {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    //};

    //int ceilingLayout[10][10] = {
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    //};
};