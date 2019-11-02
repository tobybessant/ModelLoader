#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "ModelReader.h"
#include "GLFWServices.h"
#include "GLEWServices.h"
#include "ShaderProgram.h"
#include "Mesh.h"

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

	Mesh awesomeface = Mesh();

	while (!glfw.quit()) {
		program.update();
		awesomeface.render(program.id());
		glfw.update();
	}
	glfw.destroy();
}