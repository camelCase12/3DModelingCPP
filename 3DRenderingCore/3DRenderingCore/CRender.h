#pragma once
#include <vector>
#include "Vector2D.h"
#include "Vector3D.h"

class CRender // contains a buffer of information to be drawn in the upcoming frame
{
public:
	static std::vector<Vector2D> lineStartVertices; // ArrayList like structure to store a dynamic set of lines
	static std::vector<Vector2D> lineEndVertices;
	static std::vector<Vector3D> lineColors;
	static std::vector<Vector2D> points;
	static std::vector<Vector2D> connectedPoints;
	static std::vector<std::vector<Vector2D>> polygons; // vector of polygons, being arrays of vector2ds
	static void clear();
	static void addLine(Vector2D startPoint, Vector2D endPoint);
	static void addLine(Vector2D startPoint, Vector2D endPoint, Vector3D color);
	static void addPoint(Vector2D point);
	static void addPolygon(std::vector<Vector2D> points);
	static void addConnectedPoint(Vector2D point, int index);
};