#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "ModelReader.h"
#include "GLFWServices.h"
#include "GLEWServices.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "MeshConfig.h"

std::vector<GLfloat> vertices{
	0.5f,  0.5f, -0.5f,  //0 top right
	0.5f, -0.5f, -0.5f,  //1 bottom right
	-0.5f, -0.5f, -0.5f, //2 bottom left
	-0.5f,  0.5f, -0.5f,

	0.5f,  0.5f, 0.5f,  //4 top right
	0.5f, -0.5f, 0.5f,  //5 bottom right
	-0.5f, -0.5f, 0.5f, //6 bottom left
	-0.5f,  0.5f, 0.5f
};

std::vector<GLuint> indices = {
	0, 3, 1,  // first Triangle front
	3, 2, 1,   // second Triangle

	4, 7, 0,
	7, 3, 0,

	1, 2, 5 ,
	2, 6, 5 ,

	5, 4, 0,
	0, 1, 5,

	2, 3, 7,
	7, 6, 2,

	4, 5, 7,  // first Triangle back
	7, 5, 6   // second Triangle
};

std::vector<GLfloat> colours = {
	 1.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f,
	 1.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f
};

std::vector<GLfloat> texture_coords = {
	 1.0f, 1.0f,
	 1.0f, 0.0f,
	 0.0f, 0.0f,
	 0.0f, 1.0f,

	 0.0f, 1.0f,
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 1.0f, 1.0f,
};

int main(int argc, char** argv) {
	
	// read model file
	ModelReader reader = ModelReader();
	MeshConfig config = reader.parse("./models/Pouf-obj/pouf.obj");

	/*MeshConfig config;
	config.vertices = vertices;
	config.indices = indices;
	config.texture_coords = texture_coords;
	config.colours = colours;*/

	// TODO: Check mesh is loaded correctly

	// init services
	GLFWServices glfw = GLFWServices();
	glfw.createWindow(600, 800, "VERSION 2");

	GLEWServices glew = GLEWServices();

	// create shader program
	ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
	program.use();

	Mesh awesomeface = Mesh(config);

	while (!glfw.quit()) {
		program.update();
		awesomeface.render(program.id());
		glfw.update();
	}
	glfw.destroy();
}