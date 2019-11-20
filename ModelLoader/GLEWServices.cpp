#include "GLEWServices.h"
#include <iostream>

// intialise glew
GLEWServices::GLEWServices()
{
	glewInit();
	
	std::cout << "   GLEW version: " <<
		GLEW_VERSION << "." << GLEW_VERSION_MAJOR << "." <<
		GLEW_VERSION_MINOR << "." << GLEW_VERSION_MICRO << std::endl;
}
