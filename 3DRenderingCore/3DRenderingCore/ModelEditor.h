#pragma once
#include <cmath>
#include <vector>

#include "Vector3D.h"
#include "Vector2D.h"
#include "Camera.h"
#include "CShape.h"
class ModelEditor
{
public:
	CShape currentModel;
	CShape directionalIndicator;
	Camera cam;
	std::vector<Vector3D> floatingVertices;
	std::vector<Vector2D> floatingTransformedPoints;
	std::vector<bool> floatingFrontQualifiers;
	std::vector<Vector3D> snapVertices;
	std::vector<Vector2D> snapTransformedPoints;
	std::vector<bool> snapVertexQualifiers;
	int highlightedVertex = -1;
	int dragDirection = 0; // 0 for x, 1 for y, 2 for z
	Vector3D cached3DDirectionalIndicatorPos; // also now caching the 3d version... oh boy
	Vector2D cachedDirectionalIndicatorPos; // so uh technically i should cache the 3d version and transform it. instead, i'll ask the user to stay still while dragging vertices (as they should)
	Vector2D cachedInitialProjection; // cache of the initial mouseclick vector projection onto directional indicator
	Vector2D cachedDirectionalVector; // cache of full the 2d directional vector
	float snapZ = 0;
	float cacheSnapZ = 0;
	int connectedVertexIndex1;
	bool firstIndexConnected = false;
	int connectedVertexIndex2;
	bool secondIndexConnected = false;

	void transform(Camera camIn);

	void render();

	void calculateDragDirection(Vector2D mousePos);

	void highlightVertex(Vector2D mousePos); // calculates and higlights the vertex closest to mouse. mouse coords must be -1 <= x <= 1, -1 <= y <= 1 where 1 is top and right

	void unHighlightVertex();

	void connectVertex(int index);

	void connectExistingVertices();

	void collapseConnectedVertex(bool maintainZ);

	void collapseThreeVertices();

	void collapseFourVertices();

	void createSnapVertex();

	void addVisualVertex();

	void removeVisualVertex();

	void modifyVisualVertex(Vector3D modification);

	void addFloatingVertex(Vector3D vertexIn);
	void addVertexConnection(int id); // this is going to be fun to figure out
	void transferVertices() {

	}

	ModelEditor(CShape modelIn);
	ModelEditor();
};