#include "ObjectWriter.h"
#include <iostream>
#include <fstream>

void writeFile(CShape shape, std::string file) {
	std::ofstream shapeWriter;
	shapeWriter.open(file);
	//shapeWriter.clear();

	int vertexCount = shape.getVertexSize();
	int vertexConnectionsCount = shape.vertexConnections.size();
	int linearConnectionsCount = shape.linearConnections.size();
	int linearColorsCount = shape.linearColors.size();

	shapeWriter << std::to_string(vertexCount) + "\n";
	shapeWriter << std::to_string(vertexConnectionsCount) + "\n";
	shapeWriter << std::to_string(linearConnectionsCount) + "\n";
	shapeWriter << std::to_string(linearColorsCount) + "\n";

	for (int i = 0; i < vertexCount; i++) {
		shapeWriter << shape.getVertex(i).x << "," << shape.getVertex(i).y << "," << shape.getVertex(i).z << "\n";
	}

	for (int i = 0; i < vertexConnectionsCount; i++) {
		shapeWriter << shape.vertexConnections.at(i).x << "," << shape.vertexConnections.at(i).y << "," << shape.vertexConnections.at(i).z << "\n";
	}

	for (int i = 0; i < linearConnectionsCount; i++) {
		shapeWriter << shape.linearConnections.at(i).x << "," << shape.linearConnections.at(i).y << "\n";
	}

	for (int i = 0; i < linearColorsCount; i++) {
		shapeWriter << shape.linearColors.at(i).x << "," << shape.linearColors.at(i).y << "," << shape.linearColors.at(i).z << "\n";
	}

	shapeWriter.close();
}

void createFile(std::string fileName) {
	std::ofstream outfile(fileName);
	outfile.close();
}