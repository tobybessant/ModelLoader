#include "InputManager.h"
#include <iostream>

InputManager::InputManager() { }
InputManager::~InputManager() { }

void InputManager::callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key;
	trigger(key);
}

void InputManager::addBinding(int key, const Callback& callback)
{
	keypressCallbacks[key].push_back(callback);
}

void InputManager::trigger(int key)
{
	if (keypressCallbacks.count(key) > 0) {
		for (Callback& callback : keypressCallbacks[key]) {
			callback();
		}
	}
}
