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

void removeModel(std::vector<Model>* modelStore);
void loadModel(std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore);
void addModel(ConsoleServices& console, std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore);

extern GLuint currentlyActiveModel = 0;

int main(int argc, char** argv) {
	bool exitProgram = false;

	vector<Model> models;

	string modelPath;

	// init service utilities for console and OBJ reader
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	ConsoleServices console = ConsoleServices(hConsole, &modelPath);
	ObjReader oReader = ObjReader();

	console.printStartup();

	// init GL services (models array req. in glfw for keypresses)
	GLFWServices glfw = GLFWServices(&models, &currentlyActiveModel, &console);
	

	glfw.addKeyBinding(GLFW_KEY_KP_ADD, [&]() {
		addModel(console, modelPath, oReader, models);
	});

	glfw.addKeyBinding(GLFW_KEY_KP_SUBTRACT, [&]() {
		removeModel(&models);
	});

	glfw.addKeyBinding(GLFW_KEY_F1, [&]() {
			currentlyActiveModel = 0;
	});

	glfw.addKeyBinding(GLFW_KEY_F2, [&]() {
		if (models.size() == 2)
			currentlyActiveModel = 1;
	});

	glfw.addKeyBinding(GLFW_KEY_F3, [&]() {
		if (models.size() == 3)
			currentlyActiveModel = 2;
	});

	glfw.addKeyBinding(GLFW_KEY_F4, [&]() {
		if (models.size() == 4)
			currentlyActiveModel = 3;
	});

	glfw.addKeyBinding(GLFW_KEY_F5, [&]() {
		if (models.size() == 5)
			currentlyActiveModel = 4;
	});

	glfw.createWindow(600, 800, "Model Loader");
	GLEWServices glew = GLEWServices();

	// create and use shader program
	ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
	program.use();

	// set global gl states
	glEnable(GL_DEPTH_TEST);
	
	addModel(console, modelPath, oReader, models);


	while (!glfw.shouldClose()) {

		program.update();

		for (int i = 0; i < models.size(); i++) {
			models[i].render(program.id());
		}

		glfw.update();
	}

	glfw.destroy();
}

void loadModel(std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore) {
	// read model file
	Model model = Model();
	reader.parse(modelPath, model);
	modelStore.push_back(model);
	currentlyActiveModel = modelStore.size() - 1;
}

void addModel(ConsoleServices& console, std::string& modelPath, ObjReader& reader, std::vector<Model>& modelStore)
{
	console.askForModel();
	if (reader.verifyFile(modelPath)) {
		try {
			loadModel(modelPath, reader, modelStore);
		}
		catch(...) {
			cout << "error!" << endl;
		}
	}
	else
	{
		console.error(console.InvalidFile);
		addModel(console, modelPath, reader, modelStore);
	}
}

void removeModel(std::vector<Model>* modelStore) {
	if (modelStore->size() > 0) {
		modelStore->at(modelStore->size() - 1).destroy();
		modelStore->pop_back();
		currentlyActiveModel = modelStore->size() - 1;
	}
}