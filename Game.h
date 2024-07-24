#pragma once
#ifndef GAME_H
#define GAME_H

#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "TmxParser.h"
#include "LevelMesh.h"
#include "Player.h"

#include "Enemy.h"
#include "EnemyFactory.h"
#include "Pathfinding.h"
#include "Item.h"

#include "HUD.h"
#include "SoundManager.h"

#include <iostream>
#include <string>

using namespace std;


enum GameState
{
	TITLE,
	INSTRUCTIONS,
	PLAYING,
	LEVELCOMPLETE,	
	GAMEOVER
};

class Game
{
public:

	Game();
	~Game();

	bool initialise();
	void Run();
	void Cleanup();

private:

	void ProcessInput(float deltaTime);
	void Update();
	void Render();


	// Game Screens
	void RenderTitleScreen();
	void RenderLoadingScreen();	
	void RenderLevelCompleteScreen();
	void RenderGameOverScreen();

	// Game Subsystems
	Window* m_window;
	//Camera* m_camera;
	Shader* shaderprogram;
	HUD* hud;
	Player* m_player;

	// Map Data
	TiledMap* map;
	LevelMesh* level;		
	vector<Enemy> enemies;
	vector<Item> items;

	// Game Loop 
	float deltaTime;
	float lastFrame;

	int playerHealth;
	 
	//Input handling
	bool keys[1024];		// Array to store the state of keys

	bool firstMouse;
	float lastX;
	float lastY;

	// Variables for FPS calculation
	float fpsTimeCounter;
	int fpsFrameCount;
	float fps;

	// Player shooting cooldown
	float lastShotTime;
	const float shootCooldown = 0.5f; // 0.5 seconds cooldown between shots

	// GameState
	GameState currentState;

	// Screen Textures



};




#endif // !GAME_H

