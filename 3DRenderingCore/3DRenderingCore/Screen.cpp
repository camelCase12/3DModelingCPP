#define _USE_MATH_DEFINES
#include "Screen.h"
#include "ObjectReader.h"
#include <cmath>

Camera Screen::cam = Camera(0, 0, 0, M_PI / 2.01, M_PI / 2);

std::vector<CShape> Screen::shapes;

//CShape Screen::cube = loadFile("Cube.txt");

//CShape Screen::icosahedron = loadFile("Icosahedron.txt");

ModelEditor Screen::testEditor = ModelEditor();

void Screen::transform() { // perform all necessary transformations for objects contained in the screen
	Screen::testEditor.transform(Screen::cam); // transform test editor
	for (int i = 0; i < shapes.size(); i++) { // transform all shapes in vector
		shapes.at(i).transform(Screen::cam);
	}
}

void Screen::render() { // call render function of all objects contained in the screen
	Screen::testEditor.render(); // render test editor
	for (int i = 0; i < shapes.size(); i++) { // render all shapes in vector
		shapes.at(i).render();
	}
}

bool Screen::frameRate = false;