#pragma once

#include "GLFW/glfw3.h"
#include <map>
#include <vector>
#include <string>
#include <functional>

class GLFWServices
{
public:
	GLFWServices();
	void createWindow(unsigned int height, unsigned int width, const char* windowName);
	void update();
	bool shouldClose();
	void destroy();

	using Callback = std::function<void()>;

	void addKeyBinding(GLuint key, const Callback& callback);
	void triggerKeyPress(GLuint key);
private:
	GLFWwindow* window;
	std::map<int, Callback> keypressCallbacks;
};

