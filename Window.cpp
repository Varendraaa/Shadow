#include "Window.h"

Window::Window(int width, int height, const string& title) :
		width(width),
		height(height),
		title(title),
		window(NULL)
{
	Initialize();
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::Initialize()
{
	if (!glfwInit())		//Initialise GLFW
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);			//Tells GLFW which version of OpenGL we want to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);			//In this case, we set it to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//Tells OpenGL to explicity run on core profile.

	/*Create a Window of set height and width*/
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window) //Error check if a window fails to create
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	/*Make the window's context current*/
	glfwMakeContextCurrent(window);

	/*Initialise GLAD and load its pointers*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	/*Create a Viewport. It tells OpenGL the size of the rendering window so we can display data and coordinates wrt the window*/
	glViewport(0, 0, width, height);	//First 2 params set the coords for the lower left corner, last 2 set the width and height of the window.

	/*Resize CallBack Function*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*Enables the Depth Buffer*/
	glEnable(GL_DEPTH_TEST);

	return true;
}

/*Swap Buffers*/
void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* Window::getWindow() const
{
	return window;
}

//Function to adjust the viewport size when the window is resized
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}