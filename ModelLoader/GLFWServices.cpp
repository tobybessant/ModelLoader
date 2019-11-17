#include "GLFWServices.h"
#include <iostream>

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWServices::GLFWServices(std::vector<Model>* _models, GLuint* _currentlyActiveModel, ConsoleServices* _console)
{
	models = _models;
	currentlyActiveModel = _currentlyActiveModel;
	console = _console;

	glfwInit();
	registerKeyCallbacks();
}

void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) 
{
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

void GLFWServices::triggerKeyPress(GLuint key, GLuint action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if(keypressCallbacks.count(key) > 0) {
			keypressCallbacks[key]();
		}
	}
}

void GLFWServices::registerKeyCallbacks()
{
	
	addKeyBinding(GLFW_KEY_ESCAPE, [&]() {
		glfwSetWindowShouldClose(window, true);
	});

	// model rotation
	addKeyBinding(GLFW_KEY_DOWN, [&]() {
		if(atLeastOneModel())
			models->at(*currentlyActiveModel).rotate(-0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	});

	addKeyBinding(GLFW_KEY_UP, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).rotate(0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	});

	addKeyBinding(GLFW_KEY_RIGHT, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).rotate(0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	});

	addKeyBinding(GLFW_KEY_LEFT, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).rotate(-0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	});

	// model translation
	addKeyBinding(GLFW_KEY_KP_8, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).translate(glm::vec3(0.0f, 0.0f, -1.0f));
	});

	addKeyBinding(GLFW_KEY_KP_2, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).translate(glm::vec3(0.0f, 0.0f, 1.0f));
	});
	
	addKeyBinding(GLFW_KEY_KP_4, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).translate(glm::vec3(1.0f, 0.0f, 0.0f));
	});
	
	addKeyBinding(GLFW_KEY_KP_6, [&]() {
		if (atLeastOneModel())
			models->at(*currentlyActiveModel).translate(glm::vec3(-1.0f, 0.0f, 0.0f));
	});

}

bool GLFWServices::atLeastOneModel()
{
	return models->size() > 0;
}

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// forward even onto glfwservices object
	GLFWServices* glfwServices = reinterpret_cast<GLFWServices*>(glfwGetWindowUserPointer(window));
	glfwServices->triggerKeyPress(key, action);
}