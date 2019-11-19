#pragma once
#include "Model.h"

#include <string>

class FileReader {
public:
	virtual void parse(std::string& path, Model& model) = 0;

	bool verifyFile(std::string& path) {
		FILE* fp;
		errno_t err;

		err = fopen_s(&fp, path.c_str(), "r");
		if (err == 0) {
			return true;
		}
		return false;
	}
};