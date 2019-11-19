#pragma once
#include "ILoader.h"
#include "Model.h"

#include <string>
#include <vector>
#include <map>

class DAELoader: public ILoader
{
public:
	void parse(std::string& path, Model& model);

};

