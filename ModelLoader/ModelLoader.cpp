#include <windows.h>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"

#include "GLFWServices.h"
#include "GLEWServices.h"
#include "ShaderProgram.h"
#include "Mesh.h"

#include "OBJLoader.h"
#include "DAELoader.h"
#include "ConsoleServices.h"

using namespace std;

void removeModel(std::vector<Model>* modelStore, GLuint& currentlyActiveModel);
void loadModel(std::string& modelPath, ILoader& reader, std::vector<Model>& modelStore, GLuint& currentlyActiveModel);
void addModel(ConsoleServices& console, std::string& modelPath, std::vector<Model>& modelStore, GLuint& currentlyActiveModel);

int main(int argc, char** argv) {
	// declare models store and a path variable
	string modelPath;
	vector<Model> models;
	GLuint currentlyActiveModel = 0;

	// init service utilities for console and OBJ reader
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleServices console = ConsoleServices(hConsole, &modelPath);

	// reset console
	for (int i = 0; i < 15; i++) {
		std::cout << std::endl;
	}
	// print startup message
	console.printStartup();

	// init GL services (models array req. in glfw for keypresses)
	GLFWServices glfw = GLFWServices(&models, &currentlyActiveModel);

	// keybindings that require have dependancies in the scope of this file
	glfw.addKeyBinding(GLFW_KEY_KP_ADD, [&]() {
		// reset console for new model
		for (int i = 0; i < 15; i++) {
			std::cout << std::endl;
		}
		console.printStartup();

		addModel(console, modelPath, models, currentlyActiveModel);
	});

	glfw.addKeyBinding(GLFW_KEY_KP_SUBTRACT, [&]() {
		removeModel(&models, currentlyActiveModel);
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

	// create new window and init glew services
	glfw.createWindow(600, 800, "Model Loader");
	GLEWServices glew = GLEWServices();

	// create and use shader program
	ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
	program.use();

	// set global gl states
	glEnable(GL_DEPTH_TEST);
	
	// propt for initial model file
	addModel(console, modelPath, models, currentlyActiveModel);

	// main loop to run and render models
	while (!glfw.shouldClose()) {

		// update program
		program.update();

		// loop through each added model and call render function
		for (int i = 0; i < models.size(); i++) {
			models[i].render(program.id());
		}

		// update glfw
		glfw.update();
	}

	glfw.destroy();

	console.setTextColour(console.WHITE);
}

// load a new model using the path, a FileReader, the model store to add it to, and at the end update the currently active model
void loadModel(std::string& modelPath, ILoader* loader, std::vector<Model>& modelStore, GLuint& currentlyActiveModel) {
	// parse model file into new model
	Model model = Model();
	loader->parse(modelPath, model);

	// add loaded model into array of models
	modelStore.push_back(model);

	// update currently active model to be the newly added model
	currentlyActiveModel = modelStore.size() - 1;
}

// add a model to the scene by recursively prompting the user to enter the path to a model
void addModel(ConsoleServices& console, std::string& modelPath, std::vector<Model>& modelStore, GLuint& currentlyActiveModel)
{
	console.askForModel();

	if (modelPath != "QUIT") {
		// identify file format
		string fileExt = "";
		for (size_t i = modelPath.size() - 1; i > 0; i--)
		{
			fileExt = modelPath[i] + fileExt;
			if (modelPath[i] == '.') {
				break;
			}
		}

		ILoader* loader = nullptr;
		if (fileExt.compare(".obj") == 0) {
			loader = new OBJLoader();
		}
		else if (fileExt.compare(".dae") == 0) {
			loader = new DAELoader();
		}
		else {
			console.error(console.UnsupportedFormat);
			addModel(console, modelPath, modelStore, currentlyActiveModel);
		}

		// if file format is valid, create the appropriate reader type and check file exists
		if (loader->verifyFile(modelPath)) {
			// if file exists, load into model store
			try {
				loadModel(modelPath, loader, modelStore, currentlyActiveModel);
				delete loader;
			}
			catch(...) {
				console.error(console.ReadError);
				addModel(console, modelPath, modelStore, currentlyActiveModel);
			}
		}
		else {
			// if file does not exist, log error and prompt again
			console.error(console.InvalidFile);
			addModel(console, modelPath, modelStore, currentlyActiveModel);
		}
	}
}

// remove most recently added model from the scene
void removeModel(std::vector<Model>* modelStore, GLuint& currentlyActiveModel) {
	// check there is a model to remove
	if (modelStore->size() > 0) {
		// if model to be removed is active, set active model to penultimate model
		if (currentlyActiveModel == modelStore->size()) {
			currentlyActiveModel = modelStore->size() - 1;
		}
		// deallocate model resources and pop from model store
		modelStore->at(modelStore->size() - 1).destroy();
		modelStore->pop_back();	
	}
}