#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <GLFW\glfw3.h>

class InputManager
{
public:
	using Callback = std::function<void()>;

	InputManager();
	~InputManager();

	void callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void addBinding(int key, const Callback& callback);
	void trigger(int key);

private:
	std::map<int, std::vector<Callback>> keypressCallbacks;
};

