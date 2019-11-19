#pragma once
#include "FileReader.h"
#include "Model.h"

#include <string>
#include <vector>
#include <map>

class DaeReader: public FileReader
{
public:
	void parse(std::string& path, Model& model);

};

