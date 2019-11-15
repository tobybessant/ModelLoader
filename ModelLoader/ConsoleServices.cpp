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
	askedForModel = true;

	setTextColour(GREEN);
	std::cout << "Path: ";
	std::cin >> *modelPath;

	askedForModel = false;
}

bool ConsoleServices::askingForModel()
{
	return askedForModel;
}

void ConsoleServices::printStartup()
{
	setTextColour(CYAN);

	std::cout << " _____ ______   ________  ________  _______   ___				" << std::endl;
	std::cout << "|\\   _ \\  _   \\|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\  \\				" << std::endl;
	std::cout << "\\ \\  \\\\\\__\\ \\  \\ \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\				" << std::endl;
	std::cout << " \\ \\  \\\\|__| \\  \\ \\  \\\\\\  \\ \\  \\ \\\\ \\ \\  \\_|/_\\ \\  \\			" << std::endl;
	std::cout << "  \\ \\  \\    \\ \\  \\ \\  \\\\\\  \\ \\  \\_\\\\ \\ \\  \\_|\\ \\ \\  \\____		" << std::endl;
	std::cout << "   \\ \\__\\    \\ \\__\\ \\_______\\ \\_______\\ \\_______\\ \\_______\\		" << std::endl;
	std::cout << "    \\|__|     \\|__|\\|_______|\\|_______|\\|_______|\\|_______|		" << std::endl;
	std::cout << " ___       ________  ________  ________  _______   ________		" << std::endl;
	std::cout << "|\\  \\     |\\   __  \\|\\   __  \\|\\   ___ \\|\\  ___ \\ |\\   __  \\	" << std::endl;
	std::cout << "\\ \\  \\    \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\_|\\ \\ \\   __/|\\ \\  \\|\\  \\	" << std::endl;
	std::cout << " \\ \\  \\    \\ \\  \\\\\\  \\ \\   __  \\ \\  \\ \\\\ \\ \\  \\_|/_\\ \\   _  _\\	" << std::endl;
	std::cout << "  \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\ \\  \\ \\  \\_\\\\ \\ \\  \\_|\\ \\ \\  \\\\  \\|	" << std::endl;
	std::cout << "   \\ \\_______\\ \\_______\\ \\__\\ \\__\\ \\_______\\ \\_______\\ \\__\\\\ _\\	" << std::endl;
	std::cout << "    \\|_______|\\|_______|\\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__|" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "To quit the program, please enter 'QQ'." << std::endl;

	//std::cout << "Please enter a comma-separated list of paths to the model file you would like to load:" << std::endl;
	std::cout << "Please enter the path to a model file you would like to load:" << std::endl;

	// set console colour to grey
	setTextColour(GREY);
	std::cout << "Example: models/folder1/model.obj, models/folder2/model2.obj " << std::endl;
}

void ConsoleServices::error(ERRORS errorType)
{
	setTextColour(RED);

	switch (errorType) {
	case InvalidFile:
		std::cout << "ERR: Invalid or non existant file path entered. Please try again.";
		break;
	case ReadError:
		std::cout << "ERR: Error reading file. File may be malformed or corrupt. Please use valid files only.";
		break;
	}

	std::cout << std::endl;
}
