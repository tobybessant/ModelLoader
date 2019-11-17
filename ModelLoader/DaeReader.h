#pragma once
#include "Model.h"

#include <string>
#include <vector>
#include <map>

class DaeReader
{
public:
	bool verifyFile(std::string& path);
	void parse(std::string& path, Model& model);
private:

};

