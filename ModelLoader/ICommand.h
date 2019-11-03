#pragma once

#include <iostream>

class ICommand {
public:
	virtual void execute() = 0;
};

void ICommand::execute() {
	std::cout << "No action bound.";
}

class QuitProgram : public ICommand {
public:
	virtual void execute();
};

void QuitProgram::execute() {
	ICommand::execute();  std::cout << "QUIT!";
}