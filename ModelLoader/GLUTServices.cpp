#include "GLUTServices.h"
#include <GL\freeglut_std.h>

#include "ICommand.h"

GLUTServices::GLUTServices(int argc, char** argv)
{
	glutInit(&argc, argv);
	QuitProgram quit = QuitProgram();
	inputMapping['q'] = quit;
}

void GLUTServices::keypress(char key)
{
	inputMapping[key].execute();
}
