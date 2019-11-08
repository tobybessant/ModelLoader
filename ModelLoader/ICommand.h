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
class ToggleWireframe : public ICommand {
public:
	void execute() {
		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

		if (polygonMode == GL_LINE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (polygonMode == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
};