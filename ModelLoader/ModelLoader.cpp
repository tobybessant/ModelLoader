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
	glfw.createWindow(800, 600, "VERSION 2");
}