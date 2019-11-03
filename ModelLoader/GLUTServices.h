#pragma once

#include <map>
#include <string>
#include "ICommand.h"

class GLUTServices
{
public:
	GLUTServices(int argc, char** argv);
private:
	std::map<char, ICommand> inputMapping;
	void keypress(char key);
};

