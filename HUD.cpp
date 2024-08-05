#include "HUD.h"

HUD::HUD() :
	HUDfont(nullptr)
{
	health = 100;
	ammo = 100;
}

HUD::~HUD()
{
}

void HUD::Initialise()
{
	loadTextures();
	loadFont();
}

void HUD::loadTextures()
{
	healthTexture = TextureLoader::LoadTexture("Textures/HUD/HUD_HEALTH.png");
	ammoTexture = TextureLoader::LoadTexture("Textures/HUD/HUD_AMMO.png");

	if (!healthTexture || !ammoTexture) 
	{
		std::cerr << "Failed to load one or more HUD textures" << std::endl;
	}
}

void HUD::loadFont() 
{
	ImGuiIO& io = ImGui::GetIO();
	HUDfont = io.Fonts->AddFontFromFileTTF("Textures/HUD//SpaceMission-rgyw9.otf", 24.0f);
	if (!HUDfont) 
	{
		std::cerr << "Failed to load custom font!" << std::endl;
	}
}

void HUD::updateFPS(GLfloat fps) 
{
	this->fps = fps;
}

void HUD::Render()
{
	// Start the IMGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get Window Size
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 windowSize = io.DisplaySize;

	// HUD Window
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);
	ImGui::Begin("HUD", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

	// Font size
	float fontSize = 1.0f;
	
	// Use custom font if loaded
	if (HUDfont) 
	{
		ImGui::PushFont(HUDfont);
	}

	// Set font size
	ImGui::SetWindowFontScale(fontSize);

	// Health Icon and Text
	ImGui::SetCursorPos(ImVec2(10, 660));  // Position
	ImGui::Image((void*)(intptr_t)healthTexture, ImVec2(50, 50));	// Icon Size
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(70, 675));  // Position relative to the window
	ImGui::Text(" %d", health);

	// Weapon Icon and Text
	ImGui::SetCursorPos(ImVec2(10, 600));  // Position
	ImGui::Image((void*)(intptr_t)ammoTexture, ImVec2(50, 50));
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(70, 615));  // Position relative to the window
	ImGui::Text(" %d", ammo);

	// FPS counter at the top left
	ImGui::SetCursorPos(ImVec2(10, 10));  // Position
	ImGui::Text("FPS: %.1f", fps);

	// Level counter at the top right
	ImGui::SetCursorPos(ImVec2(10, 50));  // Position
	ImGui::Text("Level: %d", currentLevel);


	// Pop custom font if used
	if (HUDfont) 
	{
		ImGui::PopFont();
	}

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void HUD::UpdateHealth(GLint health) 
{
	this->health = health;
}

void HUD::UpdateAmmo(GLint ammo) 
{
	this->ammo = ammo;
}

void HUD::updateLevel(GLint level)
{
	this->currentLevel = level;
}