#include <windows.h>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "ModelReader.h"
#include "GLFWServices.h"
#include "GLEWServices.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "MeshConfig.h"

#include "ObjReader.h"
#include "ConsoleServices.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main(int argc, char** argv) {
	// init service utilities for console and OBJ reader
	ConsoleServices console = ConsoleServices(hConsole);
	ObjReader oReader = ObjReader();

	string modelPath;

	// print startup text
	console.printStartup();

	while (modelPath != "QQ") {
		SetConsoleTextAttribute(hConsole, 2);
		cout << "Path: ";
		cin >> modelPath;

		//check model file
		//reader.verifyFile(modelPath)
		if (true) {

			// init services
			GLFWServices glfw = GLFWServices();
			glfw.createWindow(600, 800, "VERSION 2");

			GLEWServices glew = GLEWServices();
		
			// set global gl states
			//glFrontFace(GL_CCW);
			//glCullFace(GL_BACK);
			//glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			// create shader program
			ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
			program.use();

			// read model file
			Model model = Model();
			oReader.parse(modelPath, model);
			while (!glfw.quit()) {
				program.update();
				model.render(program.id());
				glfw.update();
			}
			glfw.destroy();
		}
		else {
			SetConsoleTextAttribute(hConsole, 4);
			cout << "ERR: At least one invalid/non-existant model file found. Please remove and try again." << endl;
		}
	}
}