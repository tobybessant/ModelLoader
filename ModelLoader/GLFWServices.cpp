#include "GLFWServices.h"

GLFWServices::GLFWServices()
{
	glfwInit();
}

void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) {
	window = glfwCreateWindow(width, height, windowName, NULL, NULL);
	glfwMakeContextCurrent(window);
}

void GLFWServices::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GLFWServices::quit()
{
	return glfwWindowShouldClose(window);
}

void GLFWServices::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

