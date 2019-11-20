#include "GLFWServices.h"
#include <iostream>

void keypress(GLFWwindow* window, int key, int scancode, int action, int mods);

// construct new GLFW wrapper 
GLFWServices::GLFWServices(std::vector<Model>* _models, GLuint* _currentlyActiveModel)
{
	// set constructor variables
	// models state to track user input
	models = _models;
	currentlyActiveModel = _currentlyActiveModel;

	// init glfw and assign initial key callbacks
	glfwInit();
	registerKeyCallbacks();
}

// create new window, set keypress callback, and make this window the current context
void GLFWServices::createWindow(unsigned int height, unsigned int width, const char* windowName) 
{
	window = glfwCreateWindow(width, height, windowName, NULL, NULL);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keypress);

	glfwMakeContextCurrent(window);
	std::cout << "   OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

}

// update call for glfw services 
void GLFWServices::update()
{
	// swap buffers and poll event states
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GLFWServices::shouldClose()
{
	return glfwWindowShouldClose(window);
}

// de-allocate glfw resources
void GLFWServices::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

// add a keybinding to glfw keybindings dictionary
void GLFWServices::addKeyBinding(GLuint key, const Callback& callback)
{
	keypressCallbacks[key] = callback;
}

// execute the assigned callback if present for given key
void GLFWServices::triggerKeyPress(GLuint key, GLuint action)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if(keypressCallbacks.count(key) > 0) {
			keypressCallbacks[key]();
		}
	}
}

// assign basic input callbacks to given keys
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

	// wireframe view
	addKeyBinding(GLFW_KEY_W, [&]() {
		if (!wireframeView) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframeView = true;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframeView = false;
		}
	});

}

bool GLFWServices::atLeastOneModel()
{
	return models->size() > 0;
}

// glfw keypress callback function
void keypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// forward event onto glfwservices object assigned to the window's user pointer
	GLFWServices* glfwServices = reinterpret_cast<GLFWServices*>(glfwGetWindowUserPointer(window));
	glfwServices->triggerKeyPress(key, action);
}