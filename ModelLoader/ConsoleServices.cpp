#include "ConsoleServices.h"

ConsoleServices::ConsoleServices(HANDLE& _h)
{
	h = _h;
}

void ConsoleServices::setTextColour(std::string colour)
{
	// set console colour to cyan
	if (colour == "cyan") {
		SetConsoleTextAttribute(h, 11);
	}
	if (colour == "grey") {
		SetConsoleTextAttribute(h, 8);
	}
	if (colour == "green") {
		SetConsoleTextAttribute(h, 2);
	}
}

void ConsoleServices::askForModel(std::string& modelPath)
{
	setTextColour("green");
	std::cout << "Path: ";
	std::cin >> modelPath;
}

void ConsoleServices::printStartup()
{
	setTextColour("cyan");

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
	setTextColour("grey");
	std::cout << "Example: models/folder1/model.obj, models/folder2/model2.obj " << std::endl;
}
