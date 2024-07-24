#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "Shader.h"
#include "Pathfinding.h"
#include "TmxParser.h"
#include "SoundManager.h"

enum AnimationState
{
    WALK,
    ATTACK,
    DEATH
};

struct AnimationFrame
{
    GLuint textureID;
    int duration;
};

class Enemy
{
public:
    // Constructor that creates Enemy at specified position
    Enemy(const glm::vec3& position, int health, float size, int damage,
        const vector<AnimationFrame>& defaultFrames,
        const vector<AnimationFrame>& attackFrames,
        const vector<AnimationFrame>& deathFrames);

    void render(const glm::mat4& view, const glm::mat4& projection, Shader& shader);
    void update(float deltaTime);
    void setState(AnimationState state);

    bool checkCollision(const glm::vec3& newPosition, const TileLayer& wallLayer);

    // Getters and Setters for pathfinding positions
    glm::vec3 getPosition() const { return position; }
    glm::vec3 setPosition(const glm::vec3& newPosition) { return position = newPosition; }

    // Getters and Setters for enemy combat properties
    bool isAlive() const;
    void takeDamage(int damage);

    bool isInAttackRange(const glm::vec3& targetPosition, float attackRange);
    void attackPlayer(int& playerHealth, float attackProbability, float currentTime);

private:
    // Enemy properties
    glm::vec3 position;
    glm::vec3 size;
    int health;
	int damage;
    bool isDead;

	float lastAttackTime;
	float attackCooldown;

    // Enemy animations
    AnimationState currentState;

    vector<AnimationFrame> walkFrames;
    vector<AnimationFrame> attackFrames;
    vector<AnimationFrame> deathFrames;
    bool texturesLoaded;

    int currentFrame;
    float animationTime;
    float animationSpeed;

    // Rendering
    GLuint VAO, VBO, EBO, textureID;

    //void loadTexture();
    void setupMesh();

    //void loadAnimationFrames(vector<AnimationFrame>& frames, const vector<string>& texturePaths, const vector<int>& frameDurations);
};

#endif // ENEMY_H
