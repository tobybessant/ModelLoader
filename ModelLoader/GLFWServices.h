#pragma once

#include "Model.h"
#include "ObjReader.h"
#include "GLFW/glfw3.h"
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "ConsoleServices.h"

class GLFWServices
{
public:
	GLFWServices(std::vector<Model> *_models, GLuint* _currentlyActiveModel, ConsoleServices* console);
	void createWindow(unsigned int height, unsigned int width, const char* windowName);
	void update();
	bool shouldClose();
	void destroy();

	using Callback = std::function<void()>;

	void addKeyBinding(GLuint key, const Callback& callback);
	void triggerKeyPress(GLuint key, GLuint action);
private:
	GLFWwindow* window;
	std::string* modelPath;
	std::vector<Model>* models;
	GLuint* currentlyActiveModel;

	bool wireframeView = false;

	ConsoleServices* console;

	std::map<int, Callback> keypressCallbacks;

	void registerKeyCallbacks();
	bool atLeastOneModel();
};

