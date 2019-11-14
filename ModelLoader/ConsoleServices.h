#pragma once
#include <iostream>
#include <windows.h>

class ConsoleServices
{
public:
	ConsoleServices(HANDLE& h);

	void setTextColour(std::string colour);

	void printStartup();
private:
	HANDLE h;
};

