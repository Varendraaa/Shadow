#pragma once
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include "TmxParser.h"

using namespace std;

// Node structure to represent each position in the pathfinding grid
struct Node 
{
    glm::ivec2 position; // The grid position of the node
    Node* parent;        // Pointer to the parent node (for path reconstruction)

    Node(const glm::ivec2& position, Node* parent) : position(position), parent(parent) {}
};

class Pathfinding 
{
public:
    // Constructor to initialize the grid dimensions
    Pathfinding(int width, int height);

    // Function to find the path using Breadth-First Search (BFS)
    vector<glm::ivec2> findPath(const glm::ivec2& start, const glm::ivec2& goal, const TileLayer& wallLayer);

    // Function to check if there is a line of sight between two points
    bool hasLineOfSight(const glm::ivec2& start, const glm::ivec2& goal, const TileLayer& wallLayer);


private:
    int width;  // Width of the grid
    int height; // Height of the grid

    // 2D vector to keep track of visited nodes during BFS
    vector<vector<bool>> visited;

    // Function to check if a position is valid (within bounds and not a wall)
    bool isValid(const glm::ivec2& position, const TileLayer& wallLayer);
};
