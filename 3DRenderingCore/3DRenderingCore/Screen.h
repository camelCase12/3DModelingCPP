#pragma once
#include "Camera.h"
#include "CShape.h"
#include "ModelEditor.h"
#include <vector>

class Screen // static collection of screen objects. sub-component of renderwindow targeted to maintaining actual objects rather than dealing with the abstracts of the rendering process
{
public:
	static Camera cam;
	//static CShape cube;
	//static CShape icosahedron;
	static std::vector<CShape> shapes;
	static ModelEditor testEditor;
	static bool frameRate;
	static void transform();
	static void render();
};