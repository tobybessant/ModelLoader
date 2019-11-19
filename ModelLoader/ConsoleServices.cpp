#include "ConsoleServices.h"

ConsoleServices::ConsoleServices(HANDLE& _h, std::string* _modelPath)
{
	h = _h;
	askedForModel = false;
	modelPath = _modelPath;
}

void ConsoleServices::setTextColour(TEXT_COLOURS colour)
{
	// set console colour to cyan
	SetConsoleTextAttribute(h, colour);
}

void ConsoleServices::askForModel()
{
	// set state to interrupt main loop
	askedForModel = true;

	// set text colour to yellow and print path promt
	setTextColour(YELLOW);
	std::cout << "  Path: ";

	// save user input to modelPath variable
	std::cin >> *modelPath;
	setTextColour(GREY);
	// update asking for model state as console is no longer asking for model
	askedForModel = false;
}

bool ConsoleServices::askingForModel()
{
	return askedForModel;
}

// print startup text
void ConsoleServices::printStartup()
{
	setTextColour(CYAN);

	const char* startupHeader = "\r\n    __  _______  ____  ________       __   ____  ___    ____  __________ \r\n   /  |/  / __ \\/ __ \\/ ____/ /      / /  / __ \\/   |  / __ \\/ ____/ __ \\\r\n  / /|_/ / / / / / / / __/ / /      / /  / / / / /| | / / / / __/ / /_/ /\r\n / /  / / /_/ / /_/ / /___/ /___   / /__/ /_/ / ___ |/ /_/ / /___/ _, _/ \r\n/_/  /_/\\____/_____/_____/_____/  /_____\\____/_/  |_/_____/_____/_/ |_|  \r\n                                                                         \r\n";
	std::cout << startupHeader;

	setTextColour(WHITE);
	std::cout << std::endl;
	std::cout << "  Controls:" << std::endl;
	std::cout << "  Add Model..........Keypad +" << std::endl;
	std::cout << "  Remove Model.......Keypad -" << std::endl;
	std::cout << "  Translate Model....Keypad Arrows / KP 8, KP 6, KP 2, KP 4" << std::endl;
	std::cout << "  Rotate Model.......Arrow Keys" << std::endl;
	std::cout << "  Select Model.......F1-F5" << std::endl;

	setTextColour(CYAN);
	std::cout << std::endl;
	std::cout << "  Please enter the path to a model file you would like to load (.obj | .dae):" << std::endl;
}

// log error formatted in red based on console error enum
void ConsoleServices::error(ERRORS errorType)
{
	setTextColour(RED);

	switch (errorType) {
	case InvalidFile:
		std::cout << "  ERROR: Invalid or non existant file path entered. Please try again.";
		break;
	case ReadError:
		std::cout << "  ERROR: Error reading file. File may be malformed or corrupt. Please use valid files only.";
		break;
	case UnsupportedFormat:
		std::cout << "  ERROR: Unsupported format. File formats supported: .obj, .dae";
		break;
	}

	std::cout << std::endl;
}
