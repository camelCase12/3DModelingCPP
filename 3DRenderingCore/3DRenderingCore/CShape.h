#pragma once
#include <cmath>
#include <vector>

#include "Vector3D.h"
#include "Vector2D.h"
#include "Camera.h"

class CShape
{
private:
	std::vector<Vector3D> vertices; // array of all vertices contained in the shape
public:
	Vector3D offsetPos = Vector3D(0, 0, 0); // basically the (0, 0, 0) of the shape itself, from which its vertices are relative
	std::vector<Vector3D> vertexConnections; // array indicating sets of three indices referring to a created triangle
	std::vector<Vector2D> vertexTangentials; // For each vertex, this specifies up to two other vertices that the vertex is connected to. if second index is -1, then it is a linear connection
	std::vector<Vector2D> linearConnections; // array indicating sets of two indices referring to a simple line
	std::vector<double> triangleDistanceSquareds; // array indicating the render order of triangles in the mesh, based on the squared distance to their centroid (pretty light calc)
	std::vector<Vector3D> triangleColors; // 
	std::vector<int> linearTangentials; // For each vertex, this specifies which vertex it is connected to via line.
	std::vector<Vector3D> linearColors; // array indicating different colors for each line in linearconnections
	std::vector<Vector2D> transformedPoints; // buffer of points transformed already
	Camera cam;
	std::vector<bool> frontQualifiers; // buffer of indicators for whether each index is in front of or behind the camera

	bool useBehindCulling = true;
	bool useBehindRendering = true;
	bool drawVertices = true;
	bool drawMeshOutline = true;
	bool drawLinearConnections = true;
	bool drawMesh = false;

	void translate(Vector3D translation);

	void setVertex(int index, Vector3D newVertex);

	void addVertex(Vector3D vertexIn); // note: this will convert from absolute coordinates to internal offset coordinates automatically, meaning external absolute structures need not worry about offset

	int getVertexSize(); // getters for private vertices
	Vector3D getVertex(int index);

	void transform(Camera camIn);

	Vector3D centroid();

	void render();
	void renderMesh();
	void renderMeshOutline();
	void renderLinearConnections();

	CShape(std::vector<Vector3D> vertices, std::vector<Vector3D> vertexConnections);
	CShape(std::vector<Vector3D> vertices, std::vector<Vector3D> vertexConnections, std::vector<Vector2D> linearConnections, std::vector<Vector3D> linearColors);
	CShape();
};