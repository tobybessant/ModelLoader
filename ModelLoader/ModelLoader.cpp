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

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main(int argc, char** argv) {
	string modelPath;

	// set console colour to cyan
	SetConsoleTextAttribute(hConsole, 11);

	// "Creeper-obj/Creeper.obj"
	cout << " _____ ______   ________  ________  _______   ___				" << endl;
	cout << "|\\   _ \\  _   \\|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\  \\				" << endl;
	cout << "\\ \\  \\\\\\__\\ \\  \\ \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\				" << endl;
	cout << " \\ \\  \\\\|__| \\  \\ \\  \\\\\\  \\ \\  \\ \\\\ \\ \\  \\_|/_\\ \\  \\			" << endl;
	cout << "  \\ \\  \\    \\ \\  \\ \\  \\\\\\  \\ \\  \\_\\\\ \\ \\  \\_|\\ \\ \\  \\____		" << endl;
	cout << "   \\ \\__\\    \\ \\__\\ \\_______\\ \\_______\\ \\_______\\ \\_______\\		" << endl;
	cout << "    \\|__|     \\|__|\\|_______|\\|_______|\\|_______|\\|_______|		" << endl;
	cout << " ___       ________  ________  ________  _______   ________		" << endl;
	cout << "|\\  \\     |\\   __  \\|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\	" << endl;
	cout << "\\ \\  \\    \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\|\\  \\	" << endl;
	cout << " \\ \\  \\    \\ \\  \\\\\\  \\ \\   __  \\ \\  \\ \\\\ \\ \\  \\_|/_\\ \\   _  _\\	" << endl;
	cout << "  \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\ \\  \\ \\  \\_\\\\ \\ \\  \\_|\\ \\ \\  \\\\  \\|	" << endl;
	cout << "   \\ \\_______\\ \\_______\\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\\\ _\\	" << endl;
	cout << "    \\|_______|\\|_______|\\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|" << endl;
	cout << endl;
	cout << endl;
	cout << "To quit the program, please enter 'QQ'." << endl;
	//cout << "Please enter a comma-separated list of paths to the model file you would like to load:" << endl;
	cout << "Please enter the path to a model file you would like to load:" << endl;

	// set console colour to grey
	SetConsoleTextAttribute(hConsole, 8);
	cout << "Example: models/folder1/model.obj, models/folder2/model2.obj " << endl;
	
	const char* path = "models/creeper-obj/creeper.obj";

	ObjReader oReader = ObjReader();

	while (modelPath != "QQ") {
		SetConsoleTextAttribute(hConsole, 2);
		cout << "Path: ";
		cin >> modelPath;

		//check model file
		//reader.verifyFile(modelPath)
		if (true) {
			// read model file
			Model model = oReader.parse(path);

			// init services
			GLFWServices glfw = GLFWServices();
			glfw.createWindow(600, 800, "VERSION 2");

			GLEWServices glew = GLEWServices();

			// create shader program
			ShaderProgram program = ShaderProgram("media/triangles.vert", "media/triangles.frag");
			program.use();

			model.init();

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