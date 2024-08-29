#include "Enemy.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Enemy::Enemy(const glm::vec3& position, int health, float size, int damage,
    const vector<AnimationFrame>& walkFrames,
    const vector<AnimationFrame>& attackFrames,
    const vector<AnimationFrame>& hurtFrames,
    const vector<AnimationFrame>& deathFrames) :
    position(position),
    size(glm::vec3(size, size, size)),          // Adjust the size of the enemy
    health(health),
    currentFrame(0),
    animationTime(0.0f),
    animationSpeed(0.1f),
    currentState(AnimationState::WALK),
    previousState(AnimationState::WALK),
    isDead(false),
	lastAttackTime(0.0f),
	attackCooldown(1.0f),
    damage(damage),
	walkFrames(walkFrames),
	attackFrames(attackFrames),
    hurtFrames(hurtFrames),
	deathFrames(deathFrames)
{
    setupMesh();
}




void Enemy::setupMesh() 
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

void Enemy::render(const glm::mat4& view, const glm::mat4& projection, Shader& shader)
{
    if (walkFrames.empty() || attackFrames.empty() || hurtFrames.empty() || deathFrames.empty()) return;

    glBindVertexArray(VAO);

    GLuint currentTextureID{};
    switch (currentState)
    {
    case AnimationState::WALK:
        currentTextureID = walkFrames[currentFrame].textureID;
        break;

    case AnimationState::ATTACK:
        currentTextureID = attackFrames[currentFrame].textureID;
        break;

    case AnimationState::HURT:
        currentTextureID = hurtFrames[currentFrame].textureID;
        break;

    case AnimationState::DEATH:
        currentTextureID = deathFrames[currentFrame].textureID;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, currentTextureID);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // Billboarding: Make the quad face the camera
    glm::vec3 camRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
    glm::vec3 camUp = glm::vec3(view[0][1], view[1][1], view[2][1]);
    glm::vec3 lookAt = glm::normalize(position - glm::vec3(view[3])); // Camera position is the inverse of the view matrix's translation

    glm::mat4 billboard = glm::mat4(1.0f);
    billboard[0] = glm::vec4(camRight, 0.0f);
    billboard[1] = glm::vec4(camUp, 0.0f);
    billboard[2] = glm::vec4(lookAt, 0.0f);

    model *= billboard;

    model = glm::scale(model, size);

    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Enemy::update(float deltaTime)
{
    if (walkFrames.empty() || attackFrames.empty() || hurtFrames.empty() || deathFrames.empty()) return;

    animationTime += deltaTime * 1000; // Convert deltaTime to milliseconds

    vector<AnimationFrame>& currentframes = (currentState == AnimationState::WALK) ? walkFrames :
                                            (currentState == AnimationState::ATTACK) ? attackFrames : 
                                            (currentState == AnimationState::HURT) ? hurtFrames : deathFrames;

    if (animationTime >= currentframes[currentFrame].duration)
    {
        if (currentState == AnimationState::DEATH)
        {
            // Ensure the death animation stops at the last frame
            if (currentFrame < currentframes.size() - 1)
            {
                currentFrame++;
            }
            animationTime = 0.0f;
        }
        else if (currentState == AnimationState::HURT)
        {
            // If hurt animation is complete, return to the previous state
            if (currentFrame >= currentframes.size() - 1)
            {
                setState(previousState);
            }
            else
            {
                currentFrame++;
            }
            animationTime = 0.0f;
        }
        else
        {
            currentFrame = (currentFrame + 1) % currentframes.size();
            animationTime = 0.0f;
        }
    }

    if (isDead) return;
}

void Enemy::setState(AnimationState newState)
{
    if (currentState != newState)
    {
        if (newState == AnimationState::HURT)
        {
            previousState = currentState; // Store the current state as previous state

        }

        currentState = newState;
        currentFrame = 0;
        animationTime = 0.0f;
    }
}

bool Enemy::checkCollision(const glm::vec3& newPosition, const TileLayer& wallLayer)
{
    glm::vec3 enemyMin = newPosition - size / 2.0f;
    glm::vec3 enemyMax = newPosition + size / 2.0f;

    for (int z = 0; z < wallLayer.height; ++z)
    {
        for (int x = 0; x < wallLayer.width; ++x)
        {
            if (wallLayer.data[z * wallLayer.width + x] != 0)		// There is a wall tile here
            {
                glm::vec3 wallMin(x, 0.0f, z);
                glm::vec3 wallMax(x + 1.0f, 2.0f, z + 1.0f);		// Change y value to match wall height

                if ((enemyMin.x < wallMax.x && enemyMax.x > wallMin.x) &&
                    (enemyMin.y < wallMax.y && enemyMax.y > wallMin.y) &&
                    (enemyMin.z < wallMax.z && enemyMax.z > wallMin.z))
                {
                    return true;		// Collision detected
                }
            }
        }
    }
    return false;	// No collision detected	
}

bool Enemy::isAlive() const
{
    return health > 0;
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        health = 0;
        isDead = true;
        setState(AnimationState::DEATH);
        irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
        if (soundEngine)
        {
            soundEngine->play2D("Music/p_death.ogg"); // Play death sound
        }
    }
    else if (currentState != AnimationState::HURT)
    {
        previousState = currentState;
        setState(AnimationState::HURT);
        irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
        if (soundEngine)
        {
            soundEngine->play2D("Music/p_hurt.ogg"); // Play hurt sound
        }
    }
   
}

bool Enemy::isInAttackRange(const glm::vec3& targetPosition, float attackRange)
{
    float distance = glm::distance(position, targetPosition);
    if (distance <= attackRange)                // Checks if condition is met to return true or false
    {
        return true;
    }
    return false;
}


void Enemy::attackPlayer(int& playerHealth, float attackProbability, float currentTime) 
{
    if (currentTime - lastAttackTime >= attackCooldown) 
    {
        if (rand() % 100 < attackProbability * 100)             // attackProbability is a value between 0 and 1
        { 
            playerHealth -= damage; // Adjust damage as necessary
        }
        lastAttackTime = currentTime;
        
        irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
        if (soundEngine)
        {
            soundEngine->play2D("Music/n_soldier_attack.mp3");
        }
    }
}