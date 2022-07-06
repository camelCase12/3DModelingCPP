#include "CRender.h"

std::vector<Vector2D> CRender::lineEndVertices;
std::vector<Vector2D> CRender::lineStartVertices;
std::vector<Vector3D> CRender::lineColors; // probably optimize this at some point in the future. using 3 floating point values per line just to store color is unnecessary
std::vector<Vector2D> CRender::points;
std::vector<std::vector<Vector2D>> CRender::polygons;
std::vector<Vector2D> CRender::connectedPoints;

void CRender::clear() {
	lineStartVertices.clear();
	lineEndVertices.clear();
	lineColors.clear();
	points.clear();
	connectedPoints.clear();
	polygons.clear();
}

void CRender::addLine(Vector2D startPoint, Vector2D endPoint) {
	lineStartVertices.push_back(startPoint);
	lineEndVertices.push_back(endPoint);
	lineColors.push_back(Vector3D(1, 1, 1));
}

void CRender::addLine(Vector2D startPoint, Vector2D endPoint, Vector3D color) {
	lineStartVertices.push_back(startPoint);
	lineEndVertices.push_back(endPoint);
	lineColors.push_back(color);
}

void CRender::addPoint(Vector2D point) {
	points.push_back(point);
}

void CRender::addPolygon(std::vector<Vector2D> points) {
	polygons.push_back(points);
}

void CRender::addConnectedPoint(Vector2D point, int index) {
	connectedPoints.push_back(point);
}