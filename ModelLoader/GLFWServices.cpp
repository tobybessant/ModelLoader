#include "GLFWServices.h"
#include <iostream>

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWServices::GLFWServices()
{
	glfwInit();
}

void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) {
	window = glfwCreateWindow(width, height, windowName, NULL, NULL);

	inputEvents = InputManager();

	glfwSetKeyCallback(window, inputEvents.callback);
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

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}