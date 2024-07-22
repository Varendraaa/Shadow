#pragma once


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"

class HUD
{
public:
	HUD();
	~HUD();

	void Initialise();
	void Render();

	void UpdateHealth(int health);
	void UpdateAmmo(int ammo);
	void updateFPS(float fps); 

private:
	
	GLuint healthTexture, ammoTexture;
	int health, ammo;
	float fps;

	ImFont* HUDfont;

	void loadTextures();
	void loadFont();
};