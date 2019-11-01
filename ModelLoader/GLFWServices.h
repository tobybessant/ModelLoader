#pragma once
#include "GLFW/glfw3.h"

class GLFWServices
{
public:
	GLFWServices();
	void createWindow(unsigned int height, unsigned int width, const char* windowName);
	void destroy();
private:
	GLFWwindow* window;
};

