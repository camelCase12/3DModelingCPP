#include "ModelEditor.h"
#include "visualTransform.h"
#include "CRender.h"
#include "frustumClipping.h"
#include "ObjectReader.h"

#include <iostream> // debug

void ModelEditor::calculateDragDirection(Vector2D mousePos) {
	//get all of the 2d vectors with respect to the position of the directional indicator
	Vector2D directionalIndicatorPos = directionalIndicator.transformedPoints.at(0);
	Vector2D absoluteDirectionalIndicatorX = directionalIndicator.transformedPoints.at(1);
	Vector2D directionalIndicatorX = Vector2D(absoluteDirectionalIndicatorX.x - directionalIndicatorPos.x, absoluteDirectionalIndicatorX.y - directionalIndicatorPos.y);
	Vector2D absoluteDirectionalIndicatorY = directionalIndicator.transformedPoints.at(4);
	Vector2D directionalIndicatorY = Vector2D(absoluteDirectionalIndicatorY.x - directionalIndicatorPos.x, absoluteDirectionalIndicatorY.y - directionalIndicatorPos.y);
	Vector2D absoluteDirectionalIndicatorZ = directionalIndicator.transformedPoints.at(7);
	Vector2D directionalIndicatorZ = Vector2D(absoluteDirectionalIndicatorZ.x - directionalIndicatorPos.x, absoluteDirectionalIndicatorZ.y - directionalIndicatorPos.y);
	Vector2D relativeMousePos = Vector2D(mousePos.x - directionalIndicatorPos.x, mousePos.y - directionalIndicatorPos.y);

	//cache directionalIndicatorPos
	cachedDirectionalIndicatorPos = directionalIndicatorPos;

	//calculate the shortest distances from each vector (technically ray) to the mousePos relative vector via vector projection into vector subtraction
	Vector2D xProj = visualTransform::projectVector(relativeMousePos, directionalIndicatorX);
	Vector2D xProjectedDistance = Vector2D(relativeMousePos.x - xProj.x, relativeMousePos.y - xProj.y);
	Vector2D yProj = visualTransform::projectVector(relativeMousePos, directionalIndicatorY);
	Vector2D yProjectedDistance = Vector2D(relativeMousePos.x - yProj.x, relativeMousePos.y - yProj.y);
	Vector2D zProj = visualTransform::projectVector(relativeMousePos, directionalIndicatorZ);
	Vector2D zProjectedDistance = Vector2D(relativeMousePos.x - zProj.x, relativeMousePos.y - zProj.y);
	float xLen = xProjectedDistance.Magnitude();
	float yLen = yProjectedDistance.Magnitude();
	float zLen = zProjectedDistance.Magnitude();

	//minimize and set projected distance with nested ternary
	dragDirection = (xLen < yLen && xLen < zLen && xLen > 0) ?  0 : (yLen < xLen && yLen < zLen && yLen > 0) ?  1 :  2; // 0 -> x, 1 -> y, 2 -> z for enum-mimic
	cachedInitialProjection = (xLen < yLen&& xLen < zLen) ? xProj : (yLen < xLen&& yLen < zLen) ? yProj : zProj;
	cachedDirectionalVector = (xLen < yLen&& xLen < zLen) ? directionalIndicatorX : (yLen < xLen&& yLen < zLen) ? directionalIndicatorY : directionalIndicatorZ;
	cached3DDirectionalIndicatorPos = directionalIndicator.offsetPos;
}

void ModelEditor::connectVertex(int index) {
	int closestElementIndex = 0;
	float distance = 0;
	float smallestDistance = 1.5; // necessarily off-screen
	for (int i = 0; i < currentModel.transformedPoints.size(); i++) {
		distance = currentModel.transformedPoints.at(i).Magnitude();
		if (distance < smallestDistance) {
			closestElementIndex = i;
			smallestDistance = distance;
		}
	}
	if (index == 0) {
		connectedVertexIndex1 = closestElementIndex;
		firstIndexConnected = true;
	}
	if (index == 1) {
		connectedVertexIndex2 = closestElementIndex;
		secondIndexConnected = true;
	}
}

void ModelEditor::highlightVertex(Vector2D mousePos) {
	cacheSnapZ = snapZ;
	int closestElementIndex = 0;
	float distance = 0;
	float smallestDistance = 1.5; // necessarily off-screen
	for (int i = 0; i < currentModel.transformedPoints.size(); i++) {
		distance = Vector2D(currentModel.transformedPoints.at(i).x - mousePos.x, currentModel.transformedPoints.at(i).y - mousePos.y).Magnitude();
		if (distance < smallestDistance) {
			closestElementIndex = i;
			smallestDistance = distance;
		}
	}
	highlightedVertex = closestElementIndex;
	snapZ = currentModel.getVertex(highlightedVertex).z;
	std::cout << snapZ;
}

void ModelEditor::unHighlightVertex() {
	highlightedVertex = -1;
	snapZ = cacheSnapZ;
}

void ModelEditor::createSnapVertex() {
	int closestElementIndex = 0;
	float distance = 0;
	float smallestDistance = 1.5; // necessarily off-screen
	for (int i = 0; i < snapTransformedPoints.size(); i++) {
		distance = snapTransformedPoints.at(i).Magnitude();
		if (distance < smallestDistance) {
			closestElementIndex = i;
			smallestDistance = distance;
		}
	}
	floatingVertices.push_back(snapVertices.at(closestElementIndex));
}

void ModelEditor::collapseConnectedVertex(bool maintainZ) {
	int finalIndex = floatingVertices.size() - 1;
	if (maintainZ) {
		floatingVertices.at(finalIndex).z = currentModel.getVertex(connectedVertexIndex1).z;
	}
	currentModel.addVertex(floatingVertices.at(finalIndex));
	floatingVertices.pop_back();
	int modelVertexFinalIndex = currentModel.getVertexSize() - 1;
	currentModel.vertexConnections.push_back(Vector3D(connectedVertexIndex1, connectedVertexIndex2, modelVertexFinalIndex));
	firstIndexConnected = false;
	secondIndexConnected = false;
}

void ModelEditor::connectExistingVertices() {
	int closestElementIndex = 0;
	float distance = 0;
	float smallestDistance = 1.5; // necessarily off-screen
	for (int i = 0; i < currentModel.transformedPoints.size(); i++) {
		distance = currentModel.transformedPoints.at(i).Magnitude();
		if (distance < smallestDistance) {
			closestElementIndex = i;
			smallestDistance = distance;
		}
	}
	currentModel.vertexConnections.push_back(Vector3D(connectedVertexIndex1, connectedVertexIndex2, closestElementIndex));
	firstIndexConnected = false;
	secondIndexConnected = false;
}

void ModelEditor::collapseThreeVertices() {
	int finalIndex = floatingVertices.size() - 1;
	currentModel.addVertex(floatingVertices.at(finalIndex)); // add last 3 vertices to current model
	currentModel.addVertex(floatingVertices.at(finalIndex - 1));
	currentModel.addVertex(floatingVertices.at(finalIndex - 2));
	floatingVertices.pop_back(); // remove vertices from floating vertex array
	floatingVertices.pop_back();
	floatingVertices.pop_back();
	int modelVertexFinalIndex = currentModel.getVertexSize() - 1;
	currentModel.vertexConnections.push_back(Vector3D(modelVertexFinalIndex, modelVertexFinalIndex - 1, modelVertexFinalIndex - 2));
}

void ModelEditor::collapseFourVertices() {
	int finalIndex = floatingVertices.size() - 1;
	currentModel.addVertex(floatingVertices.at(finalIndex)); // add last 3 vertices to current model
	currentModel.addVertex(floatingVertices.at(finalIndex - 1));
	currentModel.addVertex(floatingVertices.at(finalIndex - 2));
	currentModel.addVertex(floatingVertices.at(finalIndex - 3));
	floatingVertices.pop_back();
	floatingVertices.pop_back();
	floatingVertices.pop_back();
	floatingVertices.pop_back();
	int modelVertexFinalIndex = currentModel.getVertexSize() - 1;
	currentModel.vertexConnections.push_back(Vector3D(modelVertexFinalIndex, modelVertexFinalIndex - 1, modelVertexFinalIndex - 2));
	currentModel.vertexConnections.push_back(Vector3D(modelVertexFinalIndex - 3, modelVertexFinalIndex, modelVertexFinalIndex - 2));
}

void ModelEditor::addVisualVertex() {
	Vector3D dir = cam.getDirectionalVector();
	dir = dir.scalarMultiply(.3);
	Vector3D vertex = Vector3D(cam.position.x + dir.x, cam.position.y + dir.y, cam.position.z + dir.z);
	floatingVertices.push_back(vertex);
}

void ModelEditor::removeVisualVertex() {
	floatingVertices.pop_back();
}

void ModelEditor::modifyVisualVertex(Vector3D modification) {
	floatingVertices.at(floatingVertices.size() - 1).x += modification.x;
	floatingVertices.at(floatingVertices.size() - 1).y += modification.y;
	floatingVertices.at(floatingVertices.size() - 1).z += modification.z;
}

void ModelEditor::transform(Camera camIn) {
	this->cam = camIn;
	currentModel.transform(camIn); // Standard transformation of the the completed portions of the model
	floatingTransformedPoints.clear();
	for (int i = 0; i < floatingVertices.size(); i++) {
		floatingTransformedPoints.push_back(visualTransform::transformPoint(floatingVertices.at(i), camIn));
		if (visualTransform::isBehind(camIn, floatingVertices[i])) {
			floatingFrontQualifiers.push_back(false);
		}
		else {
			floatingFrontQualifiers.push_back(true);
		}
	}
	snapTransformedPoints.clear();
	
	for (int i = 0; i < snapVertices.size(); i++) {
		snapTransformedPoints.push_back(visualTransform::transformPoint(snapVertices.at(i), cam));
	}
	//remove and refactor please
	snapVertices.clear();
	snapVertexQualifiers.clear();
	Vector2D xypos = Vector2D(floor(cam.position.x), floor(cam.position.y));
	for (int i = -15; i < 15; i++) {
		for (int j = -15; j < 15; j++) {
			snapVertices.push_back(Vector3D(int(xypos.x) + i / 5.0, int(xypos.y) + j / 5.0, snapZ));
			snapVertexQualifiers.push_back(!visualTransform::isBehind(cam, snapVertices.at(snapVertices.size()-1)));
		}
	}
	//remove and refactor please
	//directional indicator transform
	if (highlightedVertex >= 0) {
		directionalIndicator.offsetPos = currentModel.getVertex(highlightedVertex); // offsets the directional indicator to the position of the highlighted vertex
		directionalIndicator.transform(camIn);
	}
	//directional indicator transform
}

void ModelEditor::render() {
	for (int i = 0; i < floatingTransformedPoints.size(); i++) {
		if (floatingFrontQualifiers.at(i)) {
			CRender::addPoint(floatingTransformedPoints.at(i));
		}
	}
	for (int i = 0; i < currentModel.transformedPoints.size(); i++) {
		if (currentModel.frontQualifiers.at(i)) {
			CRender::addPoint(currentModel.transformedPoints.at(i));
		}
	}
	for (int i = 0; i < snapVertices.size(); i++) {
		if (snapVertexQualifiers.at(i)) {
			CRender::addPoint(visualTransform::transformPoint(snapVertices.at(i), cam));
		}
	}
	if (firstIndexConnected) {
		CRender::addConnectedPoint(currentModel.transformedPoints.at(connectedVertexIndex1), 0);
	}
	if (secondIndexConnected) {
		CRender::addConnectedPoint(currentModel.transformedPoints.at(connectedVertexIndex2), 0);
	}
	currentModel.render();
	if (highlightedVertex >= 0) {
		directionalIndicator.render();
	}
}

ModelEditor::ModelEditor(CShape modelIn) {
	this->currentModel = modelIn;
	this->directionalIndicator = loadFile("DirectionalIndicator.txt");
}
ModelEditor::ModelEditor() {
	this->directionalIndicator = loadFile("DirectionalIndicator.txt");
}