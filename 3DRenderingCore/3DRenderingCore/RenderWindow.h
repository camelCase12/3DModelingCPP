#pragma once
#include <string>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "CShape.h"
#include "ModelEditor.h"

class RenderWindow
{
public:
	GLFWwindow* window; // holds the glfwwindow object

	Camera cam;

	std::vector<CShape> shapes;

	ModelEditor testEditor;

	void transformShapes();

	void renderShapes();

	void init(); // initializes all opengl parameters, then initiates loop

	void loop(); // initiated within init function, executes the graphics rendering, interaction, and animation loop

	RenderWindow(int width, int height); // only useful constructor, all info is needed
	RenderWindow(); // empty constructor for not make error
};