#pragma once
#ifndef WINDOW_H
#define WINDOW_H

/* GLFW Headers. Remember that GLAD goes before GLFW*/
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <iostream>
#include <string>

using namespace std;

class Window
{
public:
	Window(int width, int height, const string& title);
	~Window();

	bool Initialize();				//Initialise Window with preset parameters				
	void SwapBuffers();				//Swap Buffers
	void PollEvents();				//Check for Updates
	bool ShouldClose();				//Check for any actions to close window

	GLFWwindow* getWindow() const;

private:
	int width;
	int height;
	string title;
	GLFWwindow* window;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//Function to adjust the viewport size when the window is resized	

};


#endif // !WINDOW_H
