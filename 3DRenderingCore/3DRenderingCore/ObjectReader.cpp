#include "ObjectReader.h"
#include <fstream>
#include <iostream>

CShape loadFile(std::string file) {
	std::ifstream shapeLoader;
	shapeLoader.open(file);
	std::vector<std::string> output;
	std::string currentLine;
	while (std::getline(shapeLoader, currentLine)) {
		output.push_back(currentLine);
	}
	shapeLoader.close();
	//actual loading code

	std::vector<Vector3D> vertices;
	std::vector<Vector3D> vertexConnections;
	std::vector<Vector2D> linearConnections;
	std::vector<Vector3D> linearColors;
	int vertexCount = stoi(output.at(0));
	int connectionCount = stoi(output.at(1));
	int linearConnectionCount = stoi(output.at(2));
	int linearColorCount = stoi(output.at(3));

	for (int i = 4; i < vertexCount + 4; i++) {
		std::string delimiter = ",";
		std::string currentLine = output.at(i);
		std::string xString = currentLine.substr(0, currentLine.find(delimiter));
		double x = stod(xString);
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double y = stod(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double z = stod(currentLine);
		vertices.push_back(Vector3D(x, y, z));
	}

	for (int i = vertexCount + 4; i < connectionCount + vertexCount + 4; i++) {
		std::string delimiter = ",";
		std::string currentLine = output.at(i);
		double x = stoi(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double y = stoi(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double z = stoi(currentLine);
		vertexConnections.push_back(Vector3D(x, y, z));
	}

	for (int i = connectionCount + vertexCount + 4; i < connectionCount + vertexCount + linearConnectionCount + 4; i++) {
		std::string delimiter = ",";
		std::string currentLine = output.at(i);
		double x = stoi(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double y = stoi(currentLine);
		linearConnections.push_back(Vector2D(x, y));
	}
	for (int i = connectionCount + vertexCount + linearConnectionCount + 4; i < connectionCount + vertexCount + linearConnectionCount + linearColorCount + 4; i++) {
		std::string delimiter = ",";
		std::string currentLine = output.at(i);
		double x = stoi(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double y = stoi(currentLine.substr(0, currentLine.find(delimiter)));
		currentLine = currentLine.substr(currentLine.find(delimiter) + 1, currentLine.size());
		double z = stoi(currentLine);
		linearColors.push_back(Vector3D(x, y, z));
	}


	CShape loadedShape = CShape(vertices, vertexConnections, linearConnections, linearColors);
	return loadedShape;
}