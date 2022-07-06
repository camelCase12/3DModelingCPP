#pragma once
#include "Vector3D.h"
#include "Camera.h"
#include <vector>

class frustumClipping
{
public:
	static Vector3D frustumClip(Camera c, Vector3D frontPoint, Vector3D backPoint);
	static Vector3D intersectLineWithPlane(Vector3D pointOnPlane, Vector3D planeNOrmal, Vector3D pointOnLine, Vector3D lineDefinition);
	static bool isWithinFrustum(Vector3D point1, Camera c);
	static bool greaterClip(Camera c, Vector3D point1, Vector3D point2, Vector3D backpoint, Vector3D frontpoint, int planeIndex1, int planeIndex2);
	static std::vector<Vector3D> getFrustumIntersections(Camera c, Vector3D frontpoint, Vector3D backpoint);
	static bool crossesFrustum(Camera c, Vector3D frontpoint, Vector3D backpoint);
	static bool isInFront(Camera c, Vector3D v);
};