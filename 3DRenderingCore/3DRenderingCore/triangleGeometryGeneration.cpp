#define _USE_MATH_DEFINES
#include "triangleGeometryGeneration.h"
#include "visualTransform.h"
#include <iostream>
#include <cmath>

std::vector<Vector2D> triangleGeometryGeneration::getGeometry(Camera cam, std::vector<Vector3D> triangle3D, std::vector<Vector2D> triangle2D) {
	//process:
	//get list of 2d points: where points of triangle exists, where the edges of triangle intersect planes, and the corners of the frustum
	//eliminate candidate points if:
	//   1. point intersects behind frustum
	//   2. point is not on edge of triangle (line segment, not full line)
	//   3. corner of frustum is not within triangle in 2d (this one looks tricky)

	//potential solution for #3 -> create line from vector pointing from edge of frustum, intersected with plane made by triangle. check if point of intersection is inside triangle

	//initialize output vector of geometry

	
	std::vector<Vector2D> verifiedPointsInGeometry;

	//check if all points are in front
	int frontCount = 0;
	for (int i = 0; i < triangle3D.size(); i++) {
		if (frustumClipping::isInFront(cam, triangle3D.at(i))) {
			frontCount++;
		}
	}
	if (frontCount == 3) { // function will now cut abnormally; render the whole triangle if at least one vertex is in frustum, otherwise, check for frustum crosses
		for (int i = 0; i < triangle3D.size(); i++) {
			if (frustumClipping::isWithinFrustum(triangle3D.at(i), cam)) return triangle2D;
		}
		//the whole triangle is in front, but no vertices are inside the frustum, else the triangle would have returned. check if any edges of the triangle cut through the frustum, just in case
		if (frustumClipping::crossesFrustum(cam, triangle3D.at(0), triangle3D.at(1))) return triangle2D;
		if (frustumClipping::crossesFrustum(cam, triangle3D.at(1), triangle3D.at(2))) return triangle2D;
		if (frustumClipping::crossesFrustum(cam, triangle3D.at(2), triangle3D.at(0))) return triangle2D;
		//no vertices are in the frustum, and no edges cross the frustum. do not render the triangle.
		return verifiedPointsInGeometry; // returns empty array that will be thrown out
	}

	// places all valid triangle vertices in array
	for (int i = 0; i < triangle3D.size(); i++) { 
		if (frustumClipping::isWithinFrustum(triangle3D.at(i), cam)) {
			verifiedPointsInGeometry.push_back(triangle2D.at(i));
		}
	}

	if (verifiedPointsInGeometry.size() == 3) return verifiedPointsInGeometry; // cut function early if the entire triangle is already within frustum
	else if (verifiedPointsInGeometry.size() == 0) return verifiedPointsInGeometry;
	std::cout << "triangle start: " << std::endl;

	//declare 3d array of frustum edge intersections
	std::vector<Vector3D> edgeIntersections;

	std::vector<Vector3D> edgeOneIntersections = frustumClipping::getFrustumIntersections(cam, triangle3D.at(0), triangle3D.at(1));
	//std::cout << "edge one size: " << edgeOneIntersections.size();
	std::vector<Vector3D> edgeTwoIntersections = frustumClipping::getFrustumIntersections(cam, triangle3D.at(1), triangle3D.at(2));
	std::vector<Vector3D> edgeThreeIntersections = frustumClipping::getFrustumIntersections(cam, triangle3D.at(2), triangle3D.at(0));

	for (Vector3D i : edgeOneIntersections) {
		edgeIntersections.push_back(i); // potentially make constant reference alongside the transformation loop?
		//std::cout << "begin edge one: " << std::endl;
		//std::cout << i.x << ", " << i.y << ", " << i.z << std::endl;
		//std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", "<< verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}
	for (Vector3D i : edgeTwoIntersections) {
		edgeIntersections.push_back(i);
		//std::cout << "begin edge two: " << std::endl;
		//std::cout << i.x << ", " << i.y << ", " << i.z << std::endl;
		//std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}
	for (Vector3D i : edgeThreeIntersections) {
		edgeIntersections.push_back(i);
		//std::cout << "begin edge three: " << std::endl;
		//std::cout << i.x << ", " << i.y << ", " << i.z << std::endl;
		//std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}

	//transform and assign valid frustum edge intersections
	//std::cout << std::endl << "edges: " << std::endl;
	for (Vector3D i : edgeIntersections) {
		verifiedPointsInGeometry.push_back(visualTransform::transformPoint(i, cam));
		//std::cout << i.x << ", " << i.y << ", " << i.z << std::endl;
		//std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", "<< verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}

	//get directional vector for each frustum corner
	//intersect each frustum corner directional vector with plane created by 3d triangle
	// make sure each intersection is in front of frustum and within triangle
	// verify each intersection based on those conditions

	//get frustum corner vectors
	Vector3D camVector = cam.getDirectionalVector();
	Vector3D camPos = cam.position;
	Vector3D topRight = cam.getArbitraryDirectionalVector(cam.theta - cam.FOV / 2.0, cam.phi - cam.FOV / 2.0);
	Vector3D topLeft = cam.getArbitraryDirectionalVector(cam.theta + cam.FOV / 2.0, cam.phi - cam.FOV / 2.0);
	Vector3D bottomLeft = cam.getArbitraryDirectionalVector(cam.theta + cam.FOV / 2.0, cam.phi + cam.FOV / 2.0);
	Vector3D bottomRight = cam.getArbitraryDirectionalVector(cam.theta - cam.FOV / 2.0, cam.phi + cam.FOV / 2.0);

	//get plane vector for triangle
	Vector3D trianglePlaneNormal = Vector3D(visualTransform::relativize(triangle3D.at(0), triangle3D.at(1))).crossProduct(visualTransform::relativize(triangle3D.at(1), triangle3D.at(2)));
	Vector3D pointOnPlane = triangle3D.at(0);

	//get frustum corner plane intersections
	Vector3D topRightIntersection = frustumClipping::intersectLineWithPlane(pointOnPlane, trianglePlaneNormal, camPos, topRight);
	Vector2D tR2D = visualTransform::transformPoint(topRightIntersection, cam);
	std::cout << tR2D.x << ", " << tR2D.y << std::endl;
	Vector3D topLeftIntersection = frustumClipping::intersectLineWithPlane(pointOnPlane, trianglePlaneNormal, camPos, topLeft);
	Vector2D tL2D = visualTransform::transformPoint(topLeftIntersection, cam);
	std::cout << tL2D.x << ", " << tL2D.y << std::endl;
	Vector3D bottomLeftIntersection = frustumClipping::intersectLineWithPlane(pointOnPlane, trianglePlaneNormal, camPos, bottomLeft);
	Vector2D bL2D = visualTransform::transformPoint(bottomLeftIntersection, cam);
	std::cout << bL2D.x << ", " << bL2D.y << std::endl;
	Vector3D bottomRightIntersection = frustumClipping::intersectLineWithPlane(pointOnPlane, trianglePlaneNormal, camPos, bottomRight);
	Vector2D bR2D = visualTransform::transformPoint(bottomRightIntersection, cam);
	std::cout << bR2D.x << ", " << bR2D.y << std::endl;

	//check that intersections are in front of frustum, and within triangle
	//std::cout << std::endl << "corners: " << std::endl;
	if (frustumClipping::isInFront(cam, topRightIntersection) && isWithinTriangle(topRightIntersection, triangle3D)) {
		verifiedPointsInGeometry.push_back(Vector2D(1, 1)); // the linear transform of the given point is trivially known
		std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}
	if (frustumClipping::isInFront(cam, topLeftIntersection) && isWithinTriangle(topLeftIntersection, triangle3D)) {
		verifiedPointsInGeometry.push_back(Vector2D(-1, 1));
		std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}
	if (frustumClipping::isInFront(cam, bottomLeftIntersection) && isWithinTriangle(bottomLeftIntersection, triangle3D)) {
		verifiedPointsInGeometry.push_back(Vector2D(-1, -1));
		std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}
	if (frustumClipping::isInFront(cam, bottomRightIntersection) && isWithinTriangle(bottomRightIntersection, triangle3D)) {
		verifiedPointsInGeometry.push_back(Vector2D(1, -1));
		std::cout << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).x << ", " << verifiedPointsInGeometry.at(verifiedPointsInGeometry.size() - 1).y << std::endl;
	}

	
	//finally, orient the points in a clockwise rotation.\
	//calculate the centroid
	double avgX = 0;
	double avgY = 0;
	for (int i = 0; i < verifiedPointsInGeometry.size(); i++) {
		avgX += verifiedPointsInGeometry.at(i).x;
		avgY += verifiedPointsInGeometry.at(i).y;
	}
	avgX /= verifiedPointsInGeometry.size();
	avgY /= verifiedPointsInGeometry.size();
	//std::cout << "AvgX: " << avgX << ", AvgY: " << avgY;

	//calculate the angle of each point from the <1, 0> vector
	std::vector<double> angles;
	for (int i = 0; i < verifiedPointsInGeometry.size(); i++) {
		Vector2D relativized = visualTransform::relativize(Vector2D(avgX, avgY), verifiedPointsInGeometry.at(i));
		angles.push_back(visualTransform::angleBetweenVectors(Vector2D(1, 0), relativized));
		if (relativized.y < 0) {
			angles.at(i) = 2 * M_PI - angles.at(i);
		}
	}

	//for (int i = 0; i < verifiedPointsInGeometry.size(); i++) {
	//	std::cout << verifiedPointsInGeometry.at(i).x << ", " << verifiedPointsInGeometry.at(i).y << std::endl;
	//	std::cout << angles.at(i) << std::endl;
	//}

	//do a selection sort to sort points by their angles
	for (int i = 0; i < angles.size()-1; i++) {
		double smallestAngle = 2 * M_PI;
		int smallestAngleIndex = i;
		for (int j = i; j < angles.size(); j++) {
			if (angles.at(j) < smallestAngle) {
				smallestAngle = angles.at(j);
				smallestAngleIndex = j;
			}
		}
		//swap angle indices
		double tempAngle = angles.at(i);
		angles.at(i) = angles.at(smallestAngleIndex);
		angles.at(smallestAngleIndex) = tempAngle;
		//swap according verifiedPointsInGeometry indices
		Vector2D tempVector = verifiedPointsInGeometry.at(i);
		verifiedPointsInGeometry.at(i) = verifiedPointsInGeometry.at(smallestAngleIndex);
		verifiedPointsInGeometry.at(smallestAngleIndex) = tempVector;
	}

	//std::cout << std::endl << "Geometry: " << std::endl;
	//for (int i = 0; i < verifiedPointsInGeometry.size(); i++) {
		//std::cout << verifiedPointsInGeometry.at(i).x << ", " << verifiedPointsInGeometry.at(i).y << std::endl;
		//std::cout << angles.at(i) << std::endl;
	//}
	
	return verifiedPointsInGeometry;
}

bool triangleGeometryGeneration::isWithinTriangle(Vector3D point, std::vector<Vector3D> triangle) {
	//neat formula: area of triangle is simply the absolute value of the cross product of two adjacent vectors divided by two
	//also: the area of the three triangles made from PAB, PBC, PAC should be less than or equal to ABC to be inside the triangle
	double triangleArea = (Vector3D(visualTransform::relativize(triangle.at(0), triangle.at(1))).crossProduct(visualTransform::relativize(triangle.at(0), triangle.at(2)))).Magnitude() / 2.0;
	double subTriangleOne = (Vector3D(visualTransform::relativize(point, triangle.at(0))).crossProduct(visualTransform::relativize(point, triangle.at(1)))).Magnitude() / 2.0;
	double subTriangleTwo = (Vector3D(visualTransform::relativize(point, triangle.at(1))).crossProduct(visualTransform::relativize(point, triangle.at(2)))).Magnitude() / 2.0;
	double subTriangleThree = (Vector3D(visualTransform::relativize(point, triangle.at(2))).crossProduct(visualTransform::relativize(point, triangle.at(0)))).Magnitude() / 2.0;
	double ratio = (subTriangleOne + subTriangleTwo + subTriangleThree) / triangleArea;
	if (ratio <= 1.001) return true;
	return false;
}