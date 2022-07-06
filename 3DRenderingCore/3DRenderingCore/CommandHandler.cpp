#include <iostream>
#include "CommandHandler.h"
#include "GlobalStates.h"
#include "ObjectWriter.h"
#include "ObjectReader.h"

void handleCommand() {
	std::cout << "Accepting command:\n";

	std::string command;
	std::getline(std::cin, command);
	std::string delimiter = " ";
	std::string initialCommand = command.substr(0, command.find(delimiter));

	//apply initial command to a lookup table, pass the command over to a dedicated handler function
	if (initialCommand == "placementMode") {
		std::string secondaryCommand = command.substr(command.find(delimiter) + 1);
		if (secondaryCommand == "on") {
			GlobalStates::placementMode = true;
			std::cout << "placementMode: on\n";
		}
		else if (secondaryCommand == "off") {
			GlobalStates::placementMode = false;
			std::cout << "placementMode: off\n";
		}
	}
	else if (initialCommand == "frameRate") {
		std::string secondaryCommand = command.substr(command.find(delimiter) + 1);
		if (secondaryCommand == "on") {
			GlobalStates::frameRate = true;
			std::cout << "frameRate: on\n";
		}
		else if (secondaryCommand == "off") {
			GlobalStates::frameRate = false;
			std::cout << "frameRate: off\n";
		}
	}
	else if (initialCommand == "save") { // not functional right now (make possible through events?)
		std::string secondaryCommand = command.substr(command.find(delimiter) + 1);
		writeFile((*GlobalStates::renderWindow).testEditor.currentModel, secondaryCommand);
	}
	else if (initialCommand == "load") {
		std::string secondaryCommand = command.substr(command.find(delimiter) + 1);
		(*GlobalStates::renderWindow).testEditor.currentModel = loadFile(secondaryCommand);
	}
	else if (initialCommand == "setpos") { // not functional right now (make possible through events?)
		std::string secondaryCommand = command.substr(command.find(delimiter) + 1);
		std::string part1 = secondaryCommand.substr(0, secondaryCommand.find(","));
		std::string part1Leftover = secondaryCommand.substr(secondaryCommand.find(",") + 1);
		std::string part2 = part1Leftover.substr(0, part1Leftover.find(","));
		std::string part2Leftover = part1Leftover.substr(part1Leftover.find(",") + 1);
		std::string part3 = part2Leftover;
		float x, y, z;
		Vector3D pos = (*GlobalStates::renderWindow).cam.position;
		x = pos.x;
		y = pos.y;
		z = pos.z;
		if (part1 != "~") {
			x = stof(part1);
		}
		if (part2 != "~") {
			y = stof(part2);
		}
		if (part3 != "~") {
			z = stof(part3);
		}
		(*GlobalStates::renderWindow).cam.position = Vector3D(x, y, z);
	}
}