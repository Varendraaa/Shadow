#include "Pathfinding.h"

// Constructor to initialize the grid dimensions
Pathfinding::Pathfinding(int width, int height) : width(width), height(height) {}

// Function to check if a position is valid (within bounds and not a wall)
bool Pathfinding::isValid(const glm::ivec2& position, const TileLayer& wallLayer) 
{
    return (position.x >= 0 && position.x < width &&
            position.y >= 0 && position.y < height &&
            wallLayer.data[position.y * wallLayer.width + position.x] == 0);
}

// BFS algorithm to find the shortest path from start to goal
vector<glm::ivec2> Pathfinding::findPath(const glm::ivec2& start, const glm::ivec2& goal, const TileLayer& wallLayer) 
{
    vector<glm::ivec2> path;        // Vector to store the path
    queue<Node> q;                  // Queue for BFS

    // Reset the visited vector
    visited = vector<vector<bool>>(height, vector<bool>(width, false));

    // Start BFS from the starting position
    q.push(Node(start, nullptr));
    visited[start.y][start.x] = true;

    // Define the 8 possible directions for movement
    vector<glm::ivec2> directions = 
    {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, // Cardinal directions
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal directions
    };

    // BFS loop
    while (!q.empty()) 
    {
        Node current = q.front();
        q.pop();

        // If the goal is reached, backtrack to find the path
        if (current.position == goal) 
        {
            while (current.parent != nullptr) 
            {
                path.push_back(current.position);
                current = *current.parent;
            }
            reverse(path.begin(), path.end()); // Reverse to get the path from start to goal
            return path;
        }

        // Explore all possible directions
        for (const auto& dir : directions) 
        {
            glm::ivec2 nextPos = current.position + dir;
            if (isValid(nextPos, wallLayer) && !visited[nextPos.y][nextPos.x]) 
            {
                visited[nextPos.y][nextPos.x] = true;
                q.push(Node(nextPos, new Node(current)));
            }
        }
    }
    // If no path is found, return an empty path
    return path;
}

// Function to check if there is a line of sight between two points
bool Pathfinding::hasLineOfSight(const glm::ivec2& start, const glm::ivec2& goal, const TileLayer& wallLayer) 
{
    int x0 = start.x;
    int y0 = start.y;
    int x1 = goal.x;
    int y1 = goal.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) 
    {
        if (x0 == x1 && y0 == y1) return true; // Reached the goal
        if (wallLayer.data[y0 * wallLayer.width + x0] != 0) return false; // Hit a wall

        int e2 = 2 * err;
        if (e2 > -dy) 
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) 
        {
            err += dx;
            y0 += sy;
        }
    }
}

