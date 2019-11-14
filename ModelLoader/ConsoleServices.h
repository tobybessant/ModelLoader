#pragma once
#include <iostream>
#include <windows.h>

class ConsoleServices
{
public:
	ConsoleServices(HANDLE& h);

	void setTextColour(std::string colour);
	void askForModel(std::string& modelPath);

	void printStartup();

private:
	HANDLE h;
};

