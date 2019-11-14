#include "GLFWServices.h"
#include <iostream>

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWServices::GLFWServices()
{
	glfwInit();
}

void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) {
	window = glfwCreateWindow(width, height, windowName, NULL, NULL);

	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, keypress);
	glfwMakeContextCurrent(window);
}

void GLFWServices::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GLFWServices::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void GLFWServices::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLFWServices::addKeyBinding(GLuint key, const Callback& callback)
{
	keypressCallbacks[key] = callback;
}

void GLFWServices::triggerKeyPress(GLuint key)
{
	if(keypressCallbacks.count(key) > 0) {
		keypressCallbacks[key]();
	}
}

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// forward even onto glfwservices object
	GLFWServices* glfwServices = reinterpret_cast<GLFWServices*>(glfwGetWindowUserPointer(window));
	glfwServices->triggerKeyPress(key);

}