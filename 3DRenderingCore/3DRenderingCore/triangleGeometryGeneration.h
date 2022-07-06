#pragma once
#include "frustumClipping.h"
#include "Vector2D.h"
#include <vector>
class triangleGeometryGeneration
{
public:
	static std::vector<Vector2D> getGeometry(Camera cam, std::vector<Vector3D> triangle3D, std::vector<Vector2D> triangle2D);
	static bool isWithinTriangle(Vector3D point, std::vector<Vector3D> triangle);
};

