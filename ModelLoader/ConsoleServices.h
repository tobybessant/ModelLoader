#pragma once
#include <iostream>
#include <windows.h>

class ConsoleServices
{
public:
	// error and text colour enum
	enum ERRORS { InvalidFile, ReadError, UnsupportedFormat };
	enum TEXT_COLOURS { GREEN = 2, RED = 4, GREY = 8, CYAN = 11, WHITE = 15 };

	ConsoleServices(HANDLE& h, std::string* modelPath);

	void setTextColour(TEXT_COLOURS colour);
	
	void askForModel();
	bool askingForModel();

	void printStartup();

	void error(ERRORS errorType);

private:
	HANDLE h;
	std::string* modelPath;
	bool askedForModel;
};

