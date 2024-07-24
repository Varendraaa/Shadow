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
	currentState(GameState::TITLE),
	deltaTime(0.0f),
	lastFrame(0.0f),
	lastX(0.0f),
	lastY(0.0f),
	firstMouse(true)
{
	std::memset(keys, 0, sizeof(keys)); // Initialize all key states to false
}

Game::~Game()
{
	//When the item goes out of scope, delete them
	delete m_window;
	//delete m_camera;
	delete shaderprogram;
	delete level;
	delete m_player;	
	delete hud;
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

	/* Setup Platform/Renderer bindings*/
	ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");

	hud = new HUD();
	hud->Initialise();

	/* Load the Map*/
	map = new TiledMap();
	map->loadFromFile("Levels/Level1.tmx");
	
	/* Player*/
	glm::vec3 spawnPoint = map->getPlayerSpawnPosition();
	m_player = new Player(spawnPoint);
	m_player->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

	/*Shader Program*/
	shaderprogram = new Shader("shader.vert", "shader.frag");

	/*Enable Depth Test*/
	glEnable(GL_DEPTH_TEST);

	/* Create the level*/
	level = new LevelMesh();	

	/* Spawn Enemies*/
	vector<glm::vec3> enemySpawnPoints = map->getEnemySpawnPositions("Enemy");
	
	for (const auto& pos : enemySpawnPoints)
	{
		enemies.emplace_back(createSoldier(pos));
	}		

	// Load items
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
	/*Load the Title and Instruction Screen*/
	

	return true;
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

void Game::Render()
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


void Game::Update()
{
	float currentTime = glfwGetTime();

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

		// Check for Line of Sight to player
		glm::ivec2 enemyPos = glm::ivec2(enemy.getPosition().x, enemy.getPosition().z);
		
		if (pathfinder.hasLineOfSight(enemyPos, playerPos, wallLayer))
		{
			// If enemy is in attack range, attack player
			if (enemy.isInAttackRange(m_player->GetPosition(), 5.0f))	// Adjust this value as necessary for range
			{
				enemy.setState(AnimationState::ATTACK);
				enemy.attackPlayer(m_player->health, 0.1f, currentTime);					// Adjust probability as 

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

					glm::vec3 newPosition = enemy.getPosition() + direction * deltaTime * 2.0f; // Adjust speed as necessary

					// Collision Checks					
					if (!enemy.checkCollision(newPosition, wallLayer))	// Check for collision with walls
					{
						// Check for collision  with other enemies
						bool collisionWithEnemy = false;
						for (const auto& otherEnemy : enemies)
						{
							if (&enemy != &otherEnemy)
							{
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

	/* Update HUD with current player stats*/
	hud->updateFPS(fps); 
	hud->UpdateHealth(m_player->health);
	hud->UpdateAmmo(m_player->ammo);
}

void Game::RenderTitleScreen()
{
}