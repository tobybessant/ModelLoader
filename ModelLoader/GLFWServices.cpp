#include "GLFWServices.h"
#include "ICommand.h"
#include <iostream>
#include <map>

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWServices::GLFWServices()
{
	glfwInit();
}

void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) {
	window = glfwCreateWindow(width, height, windowName, NULL, NULL);
	glfwSetKeyCallback(window, keypress);
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

void GLFWServices::enableDebug(){
	
}

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::map<int, ICommand*> keyHandlers;

	QuitProgram quit = QuitProgram();
	keyHandlers[GLFW_KEY_Q] = &quit;

	keyHandlers[key]->execute();
}