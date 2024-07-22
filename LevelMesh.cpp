#include "LevelMesh.h"
#include "Texture.h"

// Constructor initializes OpenGL buffers and textures to 0
LevelMesh::LevelMesh() : VAO(0), VBO(0), EBO(0), wallTexture(0), floorTexture(0), ceilingTexture(0) 
{
	loadTextures();
    generateMesh();
}

// Destructor cleans up OpenGL resources
LevelMesh::~LevelMesh() 
{
    cleanup();
}

// Load textures for walls, floor, and ceiling
void LevelMesh::loadTextures() 
{
    wallTexture = TextureLoader::LoadTexture("Stone_Walls.png");
    floorTexture = TextureLoader::LoadTexture("LavaFloor.png");
    ceilingTexture = TextureLoader::LoadTexture("test_texture.png");
}

bool LevelMesh::isBlocked(int x, int z, const TileLayer& layout) 
{
    // Check bounds and presence of neighboring voxels
    if (x < 0 || x >= layout.width || z < 0 || z >= layout.height)
        return true;
    return layout.data[z * layout.width + x] != 0;
}

// Generate the mesh by creating cubes for each layout
void LevelMesh::generateMesh() 
{
    // Get TMX Data
    TiledMap map;
    if (!map.loadFromFile("Levels/Level1.tmx")) 
    {
        cerr << "Failed to load level data" << endl;
        return;
    }

    TileLayer floorLayer = map.getLayer("Floors");
    TileLayer wallLayer = map.getLayer("Walls");
    TileLayer ceilingLayer = map.getLayer("Floors");

    // Clear previous data, if any
    vertices.clear();
    indices.clear();
    floorIndices.clear();
    wallIndices.clear();
    ceilingIndices.clear();

    // Create the floor mesh
    for (int i = 0; i < floorLayer.height; ++i)
    {
        for (int j = 0; j < floorLayer.width; ++j)
        {
            if (floorLayer.data[i * floorLayer.width + j] != 0)
            {
                // Add only the top face of the cube
                glm::vec3 v0 = { j    , 0.0f, i };
                glm::vec3 v1 = { j + 1, 0.0f, i };
                glm::vec3 v2 = { j + 1, 0.0f, i + 1 };
                glm::vec3 v3 = { j    , 0.0f, i + 1 };
                glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

                addFace(v0, v3, v2, v1, normal, floorIndices); // Correct order for top face
            }
        }
    }

    float ceilingHeight = 2.0f;

    // Create ceiling mesh
    for (int i = 0; i < ceilingLayer.height; ++i)
    {
        for (int j = 0; j < ceilingLayer.width; ++j)
        {
            if (ceilingLayer.data[i * ceilingLayer.width + j] != 0)
            {
                // Add only the bottom face of the cube
                glm::vec3 v0 = { j    , ceilingHeight, i };
                glm::vec3 v1 = { j + 1, ceilingHeight, i };
                glm::vec3 v2 = { j + 1, ceilingHeight, i + 1 };
                glm::vec3 v3 = { j    , ceilingHeight, i + 1 };
                glm::vec3 normal = { 0.0f, -1.0f, 0.0f };

                addFace(v0, v1, v2, v3, normal, ceilingIndices); // Correct order for bottom face
            }
        }
    }

    float wallHeight = 2.0f;      // Should match ceiling height
    // Create wall mesh
    for (int i = 0; i < wallLayer.height; ++i)
    {
        for (int j = 0; j < wallLayer.width; ++j)
        {
            if (wallLayer.data[i * wallLayer.width + j] != 0)
            {
                // Back face
                if (!isBlocked(j, i + 1, wallLayer))
                {
                    glm::vec3 v0 = { j    , 0.0f, i + 1 };
                    glm::vec3 v1 = { j    , wallHeight, i + 1 };
                    glm::vec3 v2 = { j + 1, wallHeight, i + 1 };
                    glm::vec3 v3 = { j + 1, 0.0f, i + 1 };
                    glm::vec3 normal = { 0.0f, 0.0f, 1.0f };

                    addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for back face
                }

                // Front face
                if (!isBlocked(j, i - 1, wallLayer))
                {
                    glm::vec3 v0 = { j    , 0.0f, i };
                    glm::vec3 v1 = { j    , wallHeight, i };
                    glm::vec3 v2 = { j + 1, wallHeight, i };
                    glm::vec3 v3 = { j + 1, 0.0f, i };
                    glm::vec3 normal = { 0.0f, 0.0f, -1.0f };

                    addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for front face
                }

                // Left face
                if (!isBlocked(j - 1, i, wallLayer))
                {
                    glm::vec3 v0 = { j    , 0.0f, i };
                    glm::vec3 v1 = { j    , wallHeight, i };
                    glm::vec3 v2 = { j    , wallHeight, i + 1 };
                    glm::vec3 v3 = { j    , 0.0f, i + 1 };
                    glm::vec3 normal = { -1.0f, 0.0f, 0.0f };

                    addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for left face
                }

                // Right face
                if (!isBlocked(j + 1, i, wallLayer))
                {
                    glm::vec3 v0 = { j + 1, 0.0f, i };
                    glm::vec3 v1 = { j + 1, wallHeight, i };
                    glm::vec3 v2 = { j + 1, wallHeight, i + 1 };
                    glm::vec3 v3 = { j + 1, 0.0f, i + 1 };
                    glm::vec3 normal = { 1.0f, 0.0f, 0.0f };

                    addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for right face
                }
            }
        }
    }





    //// Create the floor mesh
    //for (int i = 0; i < levelHeight; ++i) 
    //{
    //    for (int j = 0; j < levelWidth; ++j) 
    //    {
    //        if (floorLayout[i][j] == 1) 
    //        {
    //            // Add only the top face of the cube
    //            glm::vec3 v0 = { j    , 0.0f, i };
    //            glm::vec3 v1 = { j + 1, 0.0f, i };
    //            glm::vec3 v2 = { j + 1, 0.0f, i + 1 };
    //            glm::vec3 v3 = { j    , 0.0f, i + 1 };
    //            glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    //            addFace(v0, v3, v2, v1, normal, floorIndices); // Correct order for top face
    //        }
    //    }
    //}

    //float ceilingHeight = 3.0f;

    //// Create ceiling mesh
    //for (int i = 0; i < levelHeight; ++i) 
    //{
    //    for (int j = 0; j < levelWidth; ++j) 
    //    {
    //        if (ceilingLayout[i][j] == 1) 
    //        {
    //            // Add only the bottom face of the cube
    //            glm::vec3 v0 = { j    , ceilingHeight, i };
    //            glm::vec3 v1 = { j + 1, ceilingHeight, i };
    //            glm::vec3 v2 = { j + 1, ceilingHeight, i + 1 };
    //            glm::vec3 v3 = { j    , ceilingHeight, i + 1 };
    //            glm::vec3 normal = { 0.0f, -1.0f, 0.0f };

    //            addFace(v0, v1, v2, v3, normal, ceilingIndices); // Correct order for bottom face
    //        }
    //    }
    //}


    //float wallHeight = 3.0f;      // Should match ceiling height
    //// Create wall mesh
    //for (int i = 0; i < levelHeight; ++i) 
    //{
    //    for (int j = 0; j < levelWidth; ++j) 
    //    {
    //        if (wallLayout[i][j] == 1) 
    //        {
    //            // Back face
    //            if (!isBlocked(j, i + 1, wallLayout)) 
    //            {
    //                glm::vec3 v0 = { j    , 0.0f, i + 1 };
    //                glm::vec3 v1 = { j    , wallHeight, i + 1 };
    //                glm::vec3 v2 = { j + 1, wallHeight, i + 1 };
    //                glm::vec3 v3 = { j + 1, 0.0f, i + 1 };
    //                glm::vec3 normal = { 0.0f, 0.0f, 1.0f };

    //                addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for back face
    //            }

    //            // Front face
    //            if (!isBlocked(j, i - 1, wallLayout)) 
    //            {
    //                glm::vec3 v0 = { j    , 0.0f, i };
    //                glm::vec3 v1 = { j    , wallHeight, i };
    //                glm::vec3 v2 = { j + 1, wallHeight, i };
    //                glm::vec3 v3 = { j + 1, 0.0f, i };
    //                glm::vec3 normal = { 0.0f, 0.0f, -1.0f };

    //                addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for front face
    //            }

    //            // Left face
    //            if (!isBlocked(j - 1, i, wallLayout)) 
    //            {
    //                glm::vec3 v0 = { j    , 0.0f, i };
    //                glm::vec3 v1 = { j    , wallHeight, i };
    //                glm::vec3 v2 = { j    , wallHeight, i + 1 };
    //                glm::vec3 v3 = { j    , 0.0f, i + 1 };
    //                glm::vec3 normal = { -1.0f, 0.0f, 0.0f };

    //                addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for left face
    //            }

    //            // Right face
    //            if (!isBlocked(j + 1, i, wallLayout)) 
    //            {
    //                glm::vec3 v0 = { j + 1, 0.0f, i };
    //                glm::vec3 v1 = { j + 1, wallHeight, i };
    //                glm::vec3 v2 = { j + 1, wallHeight, i + 1 };
    //                glm::vec3 v3 = { j + 1, 0.0f, i + 1 };
    //                glm::vec3 normal = { 1.0f, 0.0f, 0.0f };

    //                addFace(v0, v3, v2, v1, normal, wallIndices); // Correct order for right face
    //            }
    //        }
    //    }
    //}

    // Combine all indices into a single vector for the EBO
    indices.insert(indices.end(), floorIndices.begin(), floorIndices.end());
    indices.insert(indices.end(), wallIndices.begin(), wallIndices.end());
    indices.insert(indices.end(), ceilingIndices.begin(), ceilingIndices.end());

    // Setup mesh for rendering
    setupMesh();
}

// Draw the mesh
void LevelMesh::draw() 
{
    // Bind the vertex array
    glBindVertexArray(VAO);

    // Bind floor texture and draw floor part of the mesh
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawElements(GL_TRIANGLES, floorIndices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind wall texture and draw wall part of the mesh
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, (void*)(floorIndices.size() * sizeof(unsigned int)));
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind ceiling texture and draw ceiling part of the mesh
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    glDrawElements(GL_TRIANGLES, ceilingIndices.size(), GL_UNSIGNED_INT, (void*)((floorIndices.size() + wallIndices.size()) * sizeof(unsigned int)));
    glBindTexture(GL_TEXTURE_2D, 0);

    // Unbind the vertex array
    glBindVertexArray(0);
}

// Cleanup method to delete OpenGL resources
void LevelMesh::cleanup() 
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &wallTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &ceilingTexture);
}

// Setup the mesh by creating VAO, VBO, and EBO and binding vertex attributes
void LevelMesh::setupMesh() 
{
    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind and set vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Bind and set element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex attributes
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// Add a specific face of the cube at the specified position and add its indices to the target indices
void LevelMesh::addFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 normal, vector<unsigned int>& targetIndices) 
{
    // Offset for calculating indices
    GLuint baseIndex = vertices.size();

    // Define the texture coordinates of each quad
    glm::vec2 texCoords[4] = 
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    // Create and add vertices
    Vertex verticesArray[4] = 
    {
        {v0, normal, texCoords[0]},
        {v1, normal, texCoords[1]},
        {v2, normal, texCoords[2]},
        {v3, normal, texCoords[3]}
    };

    for (int i = 0; i < 4; ++i) 
    {
        vertices.push_back(verticesArray[i]);
    }

    // Indices for a single face (6 vertices forming 2 triangles)
    unsigned int faceIndices[] = 
    {
        0, 1, 2, 2, 3, 0
    };

    // Add indices to the target indices, offset by the base index
    for (int i = 0; i < 6; ++i) 
    {
        targetIndices.push_back(baseIndex + faceIndices[i]);
    }
}

/*-----------------CUBE prototype--------------------------------------------------------------*/



//// Cube Data Arrays
//// Define static cube vertex Data
//const Vertex LevelMesh::cubeVertices[24] = 
//{
//    // positions            // normals           // texture coords
//    // Front face
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
//    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
//    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
//    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
//
//    // Back face
//    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
//    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
//    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//
//    // Top face
//    {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
//    {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
//
//    // Bottom face
//    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
//    {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
//    {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
//    {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
//
//    // Right face
//    {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//    {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//    {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//    {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//
//    // Left face
//    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//    {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//    {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//    {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}
//};
//
//// Define static cube index data
//const unsigned int LevelMesh::cubeIndices[36] = 
//{
//    0, 1, 2, 2, 3, 0,           // Front face
//    4, 5, 6, 6, 7, 4,           // Back face
//    8, 9, 10, 10, 11, 8,        // Top face
//    12, 13, 14, 14, 15, 12,     // Bottom face
//    16, 17, 18, 18, 19, 16,     // Right face
//    20, 21, 22, 22, 23, 20      // Left face
//};
//
//
//// Constructor initializes OpenGL buffers and textures to 0
//LevelMesh::LevelMesh() : VAO(0), VBO(0), EBO(0), wallTexture(0), floorTexture(0), ceilingTexture(0) {}
//
//// Destructor cleans up OpenGL resources
//LevelMesh::~LevelMesh()
//{
//    // Properly delete OpenGL buffers
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteTextures(1, &wallTexture);
//    glDeleteTextures(1, &floorTexture);
//    glDeleteTextures(1, &ceilingTexture);
//}
//
//// Load textures for walls, floor, and ceiling
//void LevelMesh::loadTextures()
//{
//    wallTexture = TextureLoader::LoadTexture("stone_tex.jpg");
//    floorTexture = TextureLoader::LoadTexture("LavaFloor.png");
//    ceilingTexture = TextureLoader::LoadTexture("test_texture.png");
//}
//
//// Generate the mesh by creating cubes for each layout
//void LevelMesh::generateMesh()
//{
//    // Clear previous data, if any
//    vertices.clear();
//    indices.clear();
//    floorIndices.clear();
//    wallIndices.clear();
//    ceilingIndices.clear();
//
//    //Create the floor mesh by iterating over the floor array
//    for (int i = 0; i < levelHeight; ++i)
//    {
//        for (int j = 0; j < levelWidth; ++j)
//        {
//            if (floorLayout[i][j] == 1)
//            {
//                addCube((float)j, -1.0f, (float)i, floorIndices);
//            }
//        }
//    }
//
//    //Create Wall Mesh
//    for (int i = 0; i < levelHeight; ++i)
//    {
//        for (int j = 0; j < levelWidth; ++j)
//        {
//            if (wallLayout[i][j] == 1)
//            {
//                addCube((float)j, 0.0f, (float)i, wallIndices);
//            }
//        }
//    }
//
//    //Create Ceiling Mesh
//    for (int i = 0; i < levelHeight; ++i)
//    {
//        for (int j = 0; j < levelWidth; ++j)
//        {
//            if (ceilingLayout[i][j] == 1)
//            {
//                addCube((float)j, 1.0f, (float)i, ceilingIndices);
//            }
//        }
//    }
//
//    // Combine all indices into a single vector for the EBO
//    indices.insert(indices.end(), floorIndices.begin(), floorIndices.end());
//    indices.insert(indices.end(), wallIndices.begin(), wallIndices.end());
//    indices.insert(indices.end(), ceilingIndices.begin(), ceilingIndices.end());
//
//    // Setup mesh for rendering
//    setupMesh();
//}
//
//// Draw the mesh
//void LevelMesh::draw() 
//{
//    // Bind the vertex array
//    glBindVertexArray(VAO);
//
//    // Bind floor texture and draw floor part of the mesh
//    glBindTexture(GL_TEXTURE_2D, floorTexture);
//    glDrawElements(GL_TRIANGLES, floorIndices.size(), GL_UNSIGNED_INT, 0);
//
//    // Bind wall texture and draw wall part of the mesh
//    glBindTexture(GL_TEXTURE_2D, wallTexture);
//    glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, (void*)(floorIndices.size() * sizeof(unsigned int)));
//
//    // Bind ceiling texture and draw ceiling part of the mesh
//    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
//    glDrawElements(GL_TRIANGLES, ceilingIndices.size(), GL_UNSIGNED_INT, (void*)((floorIndices.size() + wallIndices.size()) * sizeof(unsigned int)));
//
//    // Unbind the vertex array
//    glBindVertexArray(0);
//}
//
//// Setup the mesh by creating VAO, VBO, and EBO and binding vertex attributes
//void LevelMesh::setupMesh()
//{
//    // Setup Buffers
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    // Bind Vertex Array then buffers
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//    // Vertex Data
//    //Positions
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
//    glEnableVertexAttribArray(0);
//    
//    // Normals
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//    glEnableVertexAttribArray(1);
//    
//    // Texture coordinates
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
//    glEnableVertexAttribArray(2);
//
//    glBindVertexArray(0);
//}
//
//void LevelMesh::addCube(float x, float y, float z, vector<unsigned int>& targetIndices)
//{      
//    // Offset for calculating indices
//    GLuint baseIndex = vertices.size();
//
//    // Add Vertices to the mesh, offset by the cube's position
//    for (size_t i = 0; i < 24; ++i)
//    {
//        Vertex vertex;
//        vertex.position = glm::vec3(cubeVertices[i].position.x + x, cubeVertices[i].position.y + y, cubeVertices[i].position.z + z);
//        vertex.normal = cubeVertices[i].normal;
//        vertex.texCoords = cubeVertices[i].texCoords;
//        vertices.push_back(vertex);
//    }   
//
//    // Add indices to the target indices, offset by the base index
//    for (size_t i = 0; i < 36; ++i) 
//    {
//        targetIndices.push_back(baseIndex + cubeIndices[i]);
//    }
//}