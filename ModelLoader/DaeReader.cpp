#include "DaeReader.h"
#include <iostream>
#include <regex>

bool DaeReader::verifyFile(std::string& path)
{
	return false;
}

void DaeReader::parse(const std::string& path)
{
	FILE* fp;
	errno_t err;
	char  line[500];

	std::map<std::string, std::vector<GLfloat>> dataStores;
	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;

	Model loadedModel = Model();

	std::string fileString;
	std::string fileStringCpy;

	err = fopen_s(&fp, path.c_str(), "r");
	if (err == 0) {
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			fileString += line;
		}
	}

	fileStringCpy = fileString;

	// SET DATA STORES
	std::smatch matches;
	std::regex dataSourceExpression("<source.+id=\"([\\s\\S]+?)\"[\\s\\S]*?>[\\s\\S]+?<float_array [\\s\\S]+?>([\\s\\S]+?)<\/float_array>[\\s\\S]*?");

	while (std::regex_search(fileStringCpy, matches, dataSourceExpression)) {
		std::cout << "> Processing " << matches[1] << " . . ." << std::endl;

		std::vector<GLfloat> v;
		std::string values = matches[2];

		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			v.push_back(std::stof(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		dataStores[matches[1]] =  v;
		fileStringCpy = matches.suffix();
	}

	// FETCH INDICES
	fileStringCpy = fileString;
	std::regex indicesExpression("<triangles.*>[\\s\\S]*<p>([\\s\\S]*)</p>[\\s\\S]*</triangles>");

	std::cout << "> Loading indices. . ." << std::endl;
	while (std::regex_search(fileStringCpy, matches, indicesExpression)) {
		std::string values = matches[1];

		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			indices.push_back(std::stoi(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		fileStringCpy = matches.suffix();
	}

	// LOAD INPUT DATA
	/* 
	std::cout << "> Preparing vertices. . ." << std::endl;
	while (std::regex_search(fileStringCpy, matches, indicesExpression)) {
		std::string values = matches[1];

		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			indices.push_back(std::stoi(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		fileStringCpy = matches.suffix();
	}
	*/
}
