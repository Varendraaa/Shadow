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

	void UpdateHealth(GLint health);
	void UpdateAmmo(GLint ammo);
	void updateFPS(GLfloat fps);
	void updateLevel(GLint level);

private:
	
	GLuint healthTexture, ammoTexture;
	GLint health, ammo, currentLevel;
	GLfloat fps;

	ImFont* HUDfont;

	void loadTextures();
	void loadFont();
};