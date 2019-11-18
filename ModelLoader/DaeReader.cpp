#include "DaeReader.h"
#include <string>
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
	std::vector<GLuint> vertexDefinitions;

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

	// GET VERTICES INPUTS
	std::map<std::string, std::string> vertInputs;

	std::regex vertPositions("<vertices.*id=\"(.*?)\".*>[\\s\\S]*?<input.*POSITION.*source=\"(.*?)\".*\/>[\\s\\S]*?<\/vertices>");
	while (std::regex_search(fileStringCpy, matches, vertPositions)) {
		std::string source = matches[1];
		// remove hash
		std::string source_ref = source.substr(1, source.size());

		vertInputs[source] = matches[2];

		// add match data to map
		fileStringCpy = matches.suffix();
	}

	// LOAD INPUT DATA
	fileStringCpy = fileString;
	std::map<GLuint, std::string> triInputs;

	std::regex triVertex("<triangles.*>[\\s\\S]*?<input.*VERTEX.*source=\"(.*?)\".*offset=\"(.*?)\"\/>[\\s\\S]*?<\/triangles>");
	std::regex triNormal("<triangles.*>[\\s\\S]*?<input.*NORMAL.*source=\"(.*?)\".*offset=\"(.*?)\"\/>[\\s\\S]*?<\/triangles>");
	std::regex triTexcoord("<triangles.*>[\\s\\S]*?<input.*TEXCOORD.*source=\"(.*?)\".*offset=\"(.*?)\"\/>[\\s\\S]*?<\/triangles>");

	std::vector<std::regex> triInputExpressions = {
		triNormal,
		triTexcoord,
		triVertex
	};

	std::cout << "> Preparing vertex data. . ." << std::endl;
	// get triangle inputs
	for (int i = 0; i < triInputExpressions.size(); i++) {

		while (std::regex_search(fileStringCpy, matches, triInputExpressions[i])) {
			std::string source = matches[1];
			std::string source_ref = source.substr(1, source.size());
			// remove hash
			if (vertInputs.count(source_ref) > 0 ) {
				triInputs[std::stoi(matches[2])] = vertInputs[source_ref];
			}	
			else {
				triInputs[std::stoi(matches[2])] = source_ref;
			}

			// add match data to map
			fileStringCpy = matches.suffix();
		}
		fileStringCpy = fileString;
	}
	
	// FETCH INDICES
	fileStringCpy = fileString;
	std::regex vertexDefinitionsExpression("<triangles.*>[\\s\\S]*<p>([\\s\\S]*)</p>[\\s\\S]*</triangles>");

	std::cout << "> Loading indices. . ." << std::endl;
	while (std::regex_search(fileStringCpy, matches, vertexDefinitionsExpression)) {
		std::string values = matches[1];

		char* token;
		char* nextToken = nullptr;

		token = strtok_s((char*)values.c_str(), " ", &nextToken);
		while (token != NULL) {
			vertexDefinitions.push_back(std::stoi(token));
			token = strtok_s(NULL, " ", &nextToken);
		}

		// add match data to map
		fileStringCpy = matches.suffix();
	}

	// BUILD VERTICES
	std::cout << "> Building model vertices. . ." << std::endl;
	for (int i = 0; i < vertexDefinitions.size(); i = i + triInputs.size()) {
		GLuint offset = i;

		for (int j = 0; j < triInputs.size(); j++) {
			std::string source = triInputs[j];
			std::cout << source << std::endl;
		}

		// update offset for the next set
		offset += triInputs.size();
		std::cout << "> Loaded "<< i << std::endl;
	}

}
