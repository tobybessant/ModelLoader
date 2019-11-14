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

void loadModel(std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore);

int main(int argc, char** argv) {
	// init service utilities for console and OBJ reader
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	ConsoleServices console = ConsoleServices(hConsole);
	ObjReader oReader = ObjReader();

	vector<Model> models;
	string modelPath;

	console.printStartup();

	while (modelPath != "QQ") {
		
		console.askForModel(modelPath);

		if (oReader.verifyFile(modelPath)) {

			// init GL services
			GLFWServices glfw = GLFWServices();
			glfw.createWindow(600, 800, "Model Loader");

			GLEWServices glew = GLEWServices();
		
			// set global gl states
			//glFrontFace(GL_CCW);
			//glCullFace(GL_BACK);
			//glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			// create and use shader program
			ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
			program.use();

			loadModel(modelPath, oReader, models);

			while (!glfw.quit()) {
				program.update();
				
				for (int i = 0; i < models.size(); i++) {
					models[i].render(program.id());
				}

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

void loadModel(std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore) {
	// read model file
	Model model = Model();
	reader.parse(modelPath, model);
	modelStore.push_back(model);
}