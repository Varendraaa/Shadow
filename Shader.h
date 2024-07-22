#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>

using namespace std;

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);	//Constructor
	~Shader();													//Destructor

	void Use() const;		//Sets current shader as active.

	/*Utility functions for shader management*/
	void SetBool(const string& name, bool value) const;
	void SetInt(const string& name, int value) const;
	void SetFloat(const string& name, float value) const;
	void SetVec3(const string& name, const glm::vec3& value) const;
	void SetMat4(const string& name, const glm::mat4& mat) const;

private:
	unsigned int ID;		//State

	string ReadFile(const char* filePath) const;
	void CheckCompileErrors(unsigned int shader, const string& type);
};

#endif
