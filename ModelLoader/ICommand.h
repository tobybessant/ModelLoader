#pragma once

#include <iostream>

// Base class
class ICommand {
public:
	virtual void execute() = 0;
};

class QuitProgram : public ICommand {
public:
	void execute() {
		glfwTerminate();
	}
};