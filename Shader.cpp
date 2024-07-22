//Shader Program

#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


//Constructor for the shader program
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Get Shader code from file path
	string vertexCode = ReadFile(vertexPath);
	string fragmentCode = ReadFile(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	/*Compile Vertex Shader*/
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);		//Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);		//Attach vertex shader source to vertex shader object
	glCompileShader(vertexShader);								//Compile vertex shader
	CheckCompileErrors(vertexShader, "VERTEX");					//Check for compile errors

	/*Compile Fragment Shader*/
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	//Create fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);		//Attach fragment shader source to fragment shader object
	glCompileShader(fragmentShader);							//Compile fragment shader
	CheckCompileErrors(fragmentShader, "FRAGMENT");				//Check for compile errors


	/*Create the Shader Program, where we link the shader objects we've created*/
	ID = glCreateProgram();						//Create Program
	glAttachShader(ID, vertexShader);			//Attach vertex shader Object
	glAttachShader(ID, fragmentShader);			//Attach fragment shader Object
	glLinkProgram(ID);							//Link Shaders to Program
	CheckCompileErrors(ID, "PROGRAM");			//Check for errors

	/*Clean Up Shader Code. Once they're linked to the program, they are not needed anymore*/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

Shader::~Shader()
{
	glDeleteShader(ID);
}

void Shader::Use() const
{
	glUseProgram(ID);

}

/*UTILITY FUNCTIONS*/
void Shader::SetBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

/*Inputs matrices into the Vertex Shader as Uniforms*/
void Shader::SetMat4(const string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

string Shader::ReadFile(const char* filePath) const
{
	std::ifstream in(filePath, std::ios::binary);

	if (in)
	{
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return contents.str();
	}
	throw std::runtime_error("Failed to open file: " + std::string(filePath));
}


void Shader::CheckCompileErrors(unsigned int shader, const string& type)
{
	int success;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

