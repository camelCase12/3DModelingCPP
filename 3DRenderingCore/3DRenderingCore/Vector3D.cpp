#include "Vector3D.h"
#include <cmath>

double Vector3D::Magnitude() {
	return sqrt(x * x + y * y + z * z);
}

double Vector3D::dotProduct(Vector3D otherVector) {
	return x * otherVector.x + y * otherVector.y + z * otherVector.z;
}

Vector3D Vector3D::crossProduct(Vector3D otherVector) {
	return Vector3D(y * otherVector.z - z * otherVector.y, z * otherVector.x - x * otherVector.z, x * otherVector.y - otherVector.y * x);
}

Vector3D Vector3D::scalarMultiply(double scalar) {
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D::Vector3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3D::Vector3D() { x = NULL; y = NULL; z = NULL; }