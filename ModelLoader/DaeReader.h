#pragma once
#include "Model.h"

#include <string>
#include <vector>
#include <map>

class DaeReader
{
public:
	bool verifyFile(std::string& path);
	void parse(const std::string& path, Model& model);
private:

};

