#include "GL/glew.h"
#include "GL/freeglut.h"

#include "ModelReader.h"
#include "GLFWServices.h"

int main(int argc, char** argv) {
	// init services
	GLFWServices glfw = GLFWServices();
	ModelReader reader = ModelReader();

	// read model file


	// TODO: Check mesh is loaded correctly
	glfw.createWindow(600, 800, "VERSION 2");

	while (!glfw.quit()) {
		glfw.update();
	}
}