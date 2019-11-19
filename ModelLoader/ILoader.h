#pragma once
#include "Model.h"

#include <string>

#include <string>

class ILoader {
public:
	virtual void parse(std::string& path, Model& model) = 0;

	// verify file function to check that the file entered actually exists
	bool verifyFile(std::string& path) {
		FILE* fp;
		errno_t err;

		err = fopen_s(&fp, path.c_str(), "r");
		if (err == 0) {
			return true;
		}
		return false;
	}

protected:
	// take a file path (to a specific file) and return the path to just the containing folder
	// e.g. 'models/creeper/creeper.obj' returns 'models/creeper/'
	std::string getDirectory(std::string& originalPath) {

		std::string directory;

		// find last ocurrence of /
		const size_t lastSlashIndex = originalPath.rfind('/');
		if (std::string::npos != lastSlashIndex)
		{
			// substring from 0 to last slash position
			directory = originalPath.substr(0, lastSlashIndex);
		}
		// add slash back on
		directory += '/';

		return directory;
	}
};