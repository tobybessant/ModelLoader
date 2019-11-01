#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "ModelReader.h"
#include "GLFWServices.h"
#include "GLEWServices.h"
#include "ShaderProgram.h"


int main(int argc, char** argv) {
	
	// read model file
	ModelReader reader = ModelReader();

	// TODO: Check mesh is loaded correctly
	
	// init services
	GLFWServices glfw = GLFWServices();
	glfw.createWindow(600, 800, "VERSION 2");

	GLEWServices glew = GLEWServices();

	// create shader program
	ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
	program.use();

	while (!glfw.quit()) {

		static const float purple[] = { 0.5f, 0.5f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, purple);
		glClear(GL_COLOR_BUFFER_BIT);

		program.update();
		glfw.update();
	}
}