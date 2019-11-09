#pragma once

#include "GLFW/glfw3.h"
#include "InputManager.h"

class GLFWServices
{
public:
	GLFWServices();
	void createWindow(unsigned int height, unsigned int width, const char* windowName);
	void update();
	bool quit();
	void destroy();
private:
	GLFWwindow* window;
	InputManager inputEvents;
};

