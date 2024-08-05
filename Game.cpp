#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game() :
	m_window(nullptr),
	//m_camera(nullptr),
	m_player(nullptr),
	shaderprogram(nullptr),
	level(nullptr),
	hud(nullptr),
	currentState(GameState::MENU),
	deltaTime(0.0f),
	lastFrame(0.0f),
	lastX(0.0f),
	lastY(0.0f),
	allEnemiesDead(false),
	winStateDelay(2.0f), // Delay of 2 seconds
	winStateTimer(0.0f),
	playerDead(false),
	gameOverDelay(2.0f), // Delay of 2 seconds
	gameOverTimer(0.0f),
	currentLevel(1),
	firstMouse(true)
{
	std::memset(keys, 0, sizeof(keys)); // Initialize all key states to false
}

Game::~Game()
{
	//When the item goes out of scope, delete them
	delete m_window;	
	delete shaderprogram;
	delete level;
	delete m_player;	
	delete hud;
}

void Game::loadLevel(GLint levelNumber)
{
	// Load the desired level from the specified file
	string levelPath = "Levels/Level" + to_string(levelNumber) + ".tmx";



	// Clean up the existing level data
	delete level;
	delete map;

	// Load the new level layout data from the tmx file
	map = new TiledMap();
	if (!map->loadFromFile(levelPath))
	{
		cerr << "Failed to load level " << levelPath << endl;
		return;
	}

	
	/* Player*/
	glm::vec3 spawnPoint = map->getPlayerSpawnPosition();
	m_player = new Player(spawnPoint);
	m_player->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

	/* Create the level from the layout*/
	level = new LevelMesh(levelPath);

	/* Spawn Enemies*/
	enemies.clear();
	vector<glm::vec3> enemySpawnPoints = map->getEnemySpawnPositions("Enemy");
	for (const auto& pos : enemySpawnPoints)
	{
		enemies.emplace_back(createSoldier(pos));
	}

	vector<glm::vec3> ogreSpawnPoints = map->getEnemySpawnPositions("Ogre");
	for (const auto& pos : ogreSpawnPoints)
	{
		enemies.emplace_back(createOgre(pos));
	}

	// Load items
	items.clear();
	auto healthPacks = map->getObjects("HealthPack");
	for (const auto& obj : healthPacks)
	{
		items.push_back(Item(glm::vec3(obj.x / 16, 0.0f, obj.y / 16), ItemType::HEALTH));
	}

	auto ammoPacks = map->getObjects("AmmoPack");
	for (const auto& obj : ammoPacks)
	{
		items.push_back(Item(glm::vec3(obj.x / 16, 0.0f, obj.y / 16), ItemType::AMMO));
	}
}

bool Game::initialise()
{
	 /*Create New component instances, such as windows etc. Add them here as we create new systems*/
	m_window = new Window(1280, 720, "Shadow");

	/* Tell GLFW to capture our mouse*/
	lastX = 1280.0f / 2.0f;
	lastY = 720.0f / 2.0f;
	glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Set clear color to a visible color*/
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	/* Enable blending*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Setup Dear ImGui context*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();	
	ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");

	hud = new HUD();
	hud->Initialise();

	/* Create the Shader Program*/
	shaderprogram = new Shader("shader.vert", "shader.frag");

	/*Enable Depth Test*/
	glEnable(GL_DEPTH_TEST);
	
	/*Load the Title and Instruction Screen*/
	menuTexture = TextureLoader::LoadTexture("Textures/TitleScreen.png");
	loadTexture = TextureLoader::LoadTexture("Textures/LoadingScreen.png");
	winTexture = TextureLoader::LoadTexture("Textures/LevelClear.png");
	gameOverTexture = TextureLoader::LoadTexture("Textures/GameOver.png");


	/* Load the first level*/
	currentLevel = 1;				// The first level to load
	totalLevels = 2;				// The total number of levels in the game. Change if more levels are added	
	loadLevel(currentLevel);

	return true;
}

void Game::Render()
{
	switch (currentState)
	{
	case GameState::MENU:
		RenderMenu();
		break;

	case GameState::LOAD:
		RenderLoadScreen();
		break;

	case GameState::GAMEPLAY:
		RenderGameplay();
		break;

	case GameState::WIN:
		RenderWinScreen();
		break;

	case GameState::GAMEOVER:
		RenderGameOverScreen();
		break;
	}
}

void Game::Update()
{
	switch (currentState)
	{
	case GameState::MENU:
		UpdateMenu();
		break;

	case GameState::LOAD:
		UpdateLoadScreen();
		break;

	case GameState::GAMEPLAY:
		UpdateGameplay();
		break;

	case GameState::WIN:
		UpdateWinScreen();
		break;

	case GameState::GAMEOVER:
		UpdateGameOverScreen();
		break;
	}
}

void Game::RenderMenu()
{	
	// Render the Menu Screen
	//Start the ImGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get window size
	GLint display_w, display_h;
	glfwGetFramebufferSize(m_window->getWindow(), &display_w, &display_h);

	// Render the Menu Screen
	ImGui::Begin("Title Screen", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2((GLfloat)display_w, (GLfloat)display_h));

	// UV coordinates for flipping the image
	ImVec2 uv0 = ImVec2(0.0f, 1.0f); // Bottom-left corner
	ImVec2 uv1 = ImVec2(1.0f, 0.0f); // Top-right corner

	// Render the Image
	ImGui::Image((void*)(intptr_t)menuTexture, ImVec2((GLfloat)display_w, (GLfloat)display_h), uv0, uv1);

	ImGui::End();

	// Render the ImGUI frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderLoadScreen()
{
// Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the ImGUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Get window size
    GLint display_w, display_h;
    glfwGetFramebufferSize(m_window->getWindow(), &display_w, &display_h);

    // Render the Loading Screen
    ImGui::Begin("Loading Screen", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize(ImVec2((GLfloat)display_w, (GLfloat)display_h));

    // UV coordinates for flipping the image
    ImVec2 uv0 = ImVec2(0.0f, 1.0f); // Bottom-left corner
    ImVec2 uv1 = ImVec2(1.0f, 0.0f); // Top-right corner

    // Render the Image
    ImGui::Image((void*)(intptr_t)loadTexture, ImVec2((GLfloat)display_w, (GLfloat)display_h), uv0, uv1);

    ImGui::End();

    // Render the ImGUI frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderGameplay()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use the shader Program
	shaderprogram->Use();

	/*Initialises matrices to identity matrix so that they aren't null*/
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	view = m_player->GetViewMatrix();
	projection = m_player->GetProjectionMatrix();

	/*Input matrices into the vertex shader as uniforms*/
	shaderprogram->SetMat4("view", view);
	shaderprogram->SetMat4("projection", projection);
	shaderprogram->SetMat4("model", model);
	#
	// Set ambient lighting uniform
	shaderprogram->SetVec3("ambientColor", glm::vec3(1.0f, 1.0f, 1.0f));		// White light ambient color

	shaderprogram->SetVec3("spotlight.position", m_player->GetPosition());
	shaderprogram->SetVec3("spotlight.direction", m_player->GetFront());
	shaderprogram->SetVec3("spotlight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	shaderprogram->SetVec3("spotlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	shaderprogram->SetVec3("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderprogram->SetFloat("spotlight.constant", 1.0f);
	shaderprogram->SetFloat("spotlight.linear", 0.09f);
	shaderprogram->SetFloat("spotlight.quadratic", 0.032f);
	shaderprogram->SetFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
	shaderprogram->SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(18.0f)));
	shaderprogram->SetFloat("spotlightIntensity", 1.4f); // Initialize the intensity

	level->draw();			// Render the level mesh

	for (Enemy& enemy : enemies)
	{
		enemy.render(view, projection, *shaderprogram);
	}

	// Render items
	for (auto& item : items)
	{
		item.render(view, projection, *shaderprogram);
	}

	m_player->RenderWeapon(*shaderprogram);

	// Render HUD
	hud->Render();
}

void Game::RenderWinScreen()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the Win Screen
	//Start the ImGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get window size
	GLint display_w, display_h;
	glfwGetFramebufferSize(m_window->getWindow(), &display_w, &display_h);

	// Render the Menu Screen
	ImGui::Begin("Title Screen", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2((GLfloat)display_w, (GLfloat)display_h));

	// UV coordinates for flipping the image
	ImVec2 uv0 = ImVec2(0.0f, 1.0f); // Bottom-left corner
	ImVec2 uv1 = ImVec2(1.0f, 0.0f); // Top-right corner

	// Render the Image
	ImGui::Image((void*)(intptr_t)winTexture, ImVec2((GLfloat)display_w, (GLfloat)display_h), uv0, uv1);

	ImGui::End();

	// Render the ImGUI frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderGameOverScreen()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the Game Over Screen
	//Start the ImGUI frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get window size
	GLint display_w, display_h;
	glfwGetFramebufferSize(m_window->getWindow(), &display_w, &display_h);

	// Render the Menu Screen
	ImGui::Begin("Title Screen", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2((GLfloat)display_w, (GLfloat)display_h));

	// UV coordinates for flipping the image
	ImVec2 uv0 = ImVec2(0.0f, 1.0f); // Bottom-left corner
	ImVec2 uv1 = ImVec2(1.0f, 0.0f); // Top-right corner

	// Render the Image
	ImGui::Image((void*)(intptr_t)gameOverTexture, ImVec2((GLfloat)display_w, (GLfloat)display_h), uv0, uv1);

	ImGui::End();

	// Render the ImGUI frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Game::UpdateMenu()
{
	// Update the Menu Screen
	if (keys[GLFW_KEY_ENTER])
	{
		currentState = GameState::LOAD;	
	}
}

void Game::UpdateLoadScreen()
{
	// Timer to show loading screen for a minimum duration
	static float loadStartTime = glfwGetTime();
	if (glfwGetTime() - loadStartTime >= 2.0f) // Show for at least 1 second
	{
		if (keys[GLFW_KEY_ENTER])
		{
			currentState = GameState::GAMEPLAY;
		}
	}
}

void Game::UpdateWinScreen()
{
	// Update the Win Screen
	if (keys[GLFW_KEY_ENTER]) 
	{
		// Load the next level
		if (currentLevel < totalLevels)
		{
			// Increment the level number
			currentLevel++;
		}
		loadLevel(currentLevel);

		// Reset necessary game states
		winStateTimer = 0.0f;
		allEnemiesDead = false;
		currentState = GameState::GAMEPLAY;

		//ResetGame();
		//currentState = GameState::MENU;
	}
}

void Game::UpdateGameOverScreen()
{
	// Update the Game Over Screen
	if (keys[GLFW_KEY_ENTER]) 
	{
		ResetGame();
		//currentState = GameState::MENU;
	}
}

void Game::Run()
{
	/* Sound Engine - Play Background Music*/
	irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
	if (soundEngine)
	{
		soundEngine->setSoundVolume(0.4f);
		soundEngine->play2D("Music/Background.mp3", true);
	}

	while (!m_window->ShouldClose())
	{
		// Timing for FPS reasons
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// FPS Calculation
		fpsTimeCounter += deltaTime;
		fpsFrameCount++;
		if (fpsTimeCounter >= 1.0f)
		{
			fps = fpsFrameCount / fpsTimeCounter;
			fpsFrameCount = 0;
			fpsTimeCounter = 0.0f;
		}

		// Process Input for all States
		ProcessInput(deltaTime);		

		Update();
		Render();

		m_window->SwapBuffers();
		m_window->PollEvents();
	}
}

void Game::Cleanup()
{
	// Clean up game resources
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();		
}


void Game::ProcessInput(float deltaTime)
{
	/* Get the GLFW window*/
	GLFWwindow* glfwWindow = m_window->getWindow();

	/* Update key states*/
	keys[GLFW_KEY_W] = glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS;
	keys[GLFW_KEY_S] = glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS;
	keys[GLFW_KEY_A] = glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS;
	keys[GLFW_KEY_D] = glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS;
	keys[GLFW_KEY_ESCAPE] = glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	keys[GLFW_MOUSE_BUTTON_1] = glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
	keys[GLFW_KEY_ENTER] = glfwGetKey(glfwWindow, GLFW_KEY_ENTER) == GLFW_PRESS;

	if (keys[GLFW_KEY_ESCAPE])
	{
		glfwSetWindowShouldClose(glfwWindow, true);			// Close the window if the escape key is pressed
	}
		
	/* Process Player Input with collision detection*/
	if (currentState == GameState::GAMEPLAY)
	{
		TileLayer wallLayer = map->getLayer("Walls");
		m_player->ProcessPlayerInput(keys, deltaTime, wallLayer);

		double xpos, ypos;
		glfwGetCursorPos(glfwWindow, &xpos, &ypos);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		m_player->ProcessMouseMovement(xoffset, yoffset);
	}
}

void Game::UpdateGameplay()
{
	float currentTime = glfwGetTime();
	bool allEnemiesDead = true;

	// Initial Pathfinding Setup
	glm::ivec2 playerPos = glm::ivec2(m_player->GetPosition().x, m_player->GetPosition().z);
	TileLayer wallLayer = map->getLayer("Walls");
	Pathfinding pathfinder(wallLayer.width, wallLayer.height);

	//Enemy Behaviour and State Machine
	for (auto& enemy : enemies) 
	{
		// Skip Dead Enemies
		if (!enemy.isAlive())
		{
			enemy.setState(AnimationState::DEATH);
			enemy.update(deltaTime);
			continue;
		}

		allEnemiesDead = false;

		// Check for Line of Sight to player
		glm::ivec2 enemyPos = glm::ivec2(enemy.getPosition().x, enemy.getPosition().z);
		
		if (pathfinder.hasLineOfSight(enemyPos, playerPos, wallLayer))
		{
			// If enemy is in attack range, attack player
			if (enemy.isInAttackRange(m_player->GetPosition(), 5.0f))		// Adjust this value as necessary for attack range
			{
				enemy.setState(AnimationState::ATTACK);
				enemy.attackPlayer(m_player->health, 0.1f, currentTime);	// Adjust attack probability as necessary

			}

			// Otherwise, move towards player
			else
			{
				enemy.setState(AnimationState::WALK);

				vector<glm::ivec2> path = pathfinder.findPath(enemyPos, playerPos, wallLayer);
				if (!path.empty())
				{
					glm::ivec2 nextPos = path.front();
					glm::vec3 targetPos = glm::vec3(nextPos.x, enemy.getPosition().y, nextPos.y);
					glm::vec3 direction = glm::normalize(targetPos - enemy.getPosition());

					glm::vec3 newPosition = enemy.getPosition() + direction * deltaTime * 2.0f; // Adjust movement speed as necessary

					// Collision Checks					
					if (!enemy.checkCollision(newPosition, wallLayer))	// Check for collision with walls
					{
						// Check for collision  with other enemies
						bool collisionWithEnemy = false;
						for (const auto& otherEnemy : enemies)
						{
							if (&enemy != &otherEnemy)
							{
								// Check if the enemy has reached or passed the target position
								if (glm::distance(newPosition, otherEnemy.getPosition()) < 1.0f)
								{
									float distance = glm::length(otherEnemy.getPosition() - newPosition);
									if (distance < 1.0f) // Adjust this threshold as necessary
									{
										collisionWithEnemy = true;
										break;
									}
								}
							}
						}
						if (!collisionWithEnemy)
						{
							enemy.setPosition(newPosition);
						}
					}
				}
			}
		}		
		enemy.update(deltaTime);
	}

	// If all enemies are dead, start the win state timer
	if (allEnemiesDead) 
	{
		winStateTimer += deltaTime;
		if (winStateTimer >= winStateDelay) 
		{
			currentState = GameState::WIN;
		}
	}
	else 
	{
		winStateTimer = 0.0f;
	}

	 // Player shooting logic
	if (keys[GLFW_MOUSE_BUTTON_1] && glfwGetTime() - lastShotTime >= shootCooldown) 
	{
		glm::vec3 playerPos = m_player->GetPosition();
		Enemy* nearestEnemy = nullptr;
		float nearestDistance = 5.0f; // Adjust shooting range as necessary

		for (auto& enemy : enemies) 
		{
			if (enemy.isAlive()) 
			{
				float distance = glm::length(enemy.getPosition() - playerPos);
				if (distance < nearestDistance) 
				{
					nearestDistance = distance;
					nearestEnemy = &enemy;
				}
			}
		}

		if (nearestEnemy) 
		{
			nearestEnemy->takeDamage(20); // Adjust damage value as necessary
		}

		lastShotTime = glfwGetTime(); // Update the last shot time
	}

	// Player picks up items
	glm::vec3 playeritemPos = m_player->GetPosition();
	auto it = items.begin();
	while (it != items.end()) 
	{
		if (glm::distance(it->getPosition(), playeritemPos) < 1.0f) 
		{
			if (it->getType() == ItemType::HEALTH) 
			{
				m_player->health = std::min(m_player->health + 20, 100);
				irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
				if (soundEngine) 
				{
					soundEngine->play2D("Music/p_med_kit.mp3");
				}
			}
			else if (it->getType() == ItemType::AMMO) 
			{
				m_player->ammo += 10;
				irrklang::ISoundEngine* soundEngine = SoundManager::getInstance().getSoundEngine();
				if (soundEngine)
				{
					soundEngine->play2D("Music/p_ammo.ogg");					
				}
			}
			it = items.erase(it); // Remove the item from the list
		}
		else 
		{
			++it;
		}
	}

	// Check game over conditions
	if (m_player->health <= 0) 
	{
		playerDead = true;
		gameOverTimer += deltaTime;
		if (gameOverTimer >= gameOverDelay) 
		{
			currentState = GameState::GAMEOVER;
		}
	}
	else 
	{
		playerDead = false;
		gameOverTimer = 0.0f;
	}
	
	/* Update HUD with current player stats*/
	hud->updateFPS(fps); 
	hud->UpdateHealth(m_player->health);
	hud->UpdateAmmo(m_player->ammo);
	hud->updateLevel(currentLevel);
}

void Game::ResetGame() 
{
	// Reset player state
	m_player->health = 100;
	m_player->ammo = 100;	
	glm::vec3 spawnPoint = map->getPlayerSpawnPosition();
	m_player = new Player(spawnPoint);
	m_player->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

	// Reset enemies
	enemies.clear();
	vector<glm::vec3> enemySpawnPoints = map->getEnemySpawnPositions("Enemy");
	for (const auto& pos : enemySpawnPoints)
	{
		enemies.emplace_back(createSoldier(pos));
	}

	// Reset items
	items.clear();	
	auto healthPacks = map->getObjects("HealthPack");
	for (const auto& obj : healthPacks)
	{
		items.push_back(Item(glm::vec3(obj.x / 16, 0.0f, obj.y / 16), ItemType::HEALTH));
	}
	auto ammoPacks = map->getObjects("AmmoPack");
	for (const auto& obj : ammoPacks)
	{
		items.push_back(Item(glm::vec3(obj.x / 16, 0.0f, obj.y / 16), ItemType::AMMO));
	}

	// Reset timers and states
	lastShotTime = 0.0f;
	winStateTimer = 0.0f;
	allEnemiesDead = false;

	// Reset HUD
	hud->UpdateHealth(m_player->health);
	hud->UpdateAmmo(m_player->ammo);

	// Set the game state to gameplay
	currentState = GameState::GAMEPLAY;
}