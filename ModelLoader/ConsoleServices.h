#pragma once
#include <iostream>
#include <windows.h>

class ConsoleServices
{
public:
	// error and text colour enum
	enum ERRORS { InvalidFile, ReadError, UnsupportedFormat };
	enum TEXT_COLOURS { GREEN = 10, RED = 12, GREY = 8, CYAN = 11, YELLOW = 14, WHITE = 15 };

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

