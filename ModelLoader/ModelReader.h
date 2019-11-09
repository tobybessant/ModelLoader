#pragma once

#include "MeshConfig.h"

#include <GL\glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ModelReader
{
public:
	bool verifyFile(std::string& path);
	MeshConfig parse(std::string& path);
	std::vector<std::string> split(const std::string& s, char delimiter);
private:
};

//vector<string> split(const string& s, char delimiter);
//void parse(char* path);
