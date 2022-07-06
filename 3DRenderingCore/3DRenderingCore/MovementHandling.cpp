#include "MovementHandling.h"
#include "CommandHandler.h"
#include "GlobalStates.h"
#include "RenderWindow.h"
#include "visualTransform.h"
#include <cmath>

double MovementHandling::mouseX = 0;
double MovementHandling::mouseY = 0;

Vector2D MovementHandling::getMouseUnit(GLFWwindow* window) {
    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);
    //transform mousepos to fit within -1 <= n <= 1 parameters used in rendering
    float mouseXUnit = mouseX / width;
    float mouseYUnit = mouseY / height;
    mouseXUnit *= 2;
    mouseYUnit *= 2;
    mouseXUnit -= 1;
    mouseYUnit -= 1;
    mouseYUnit *= -1;
    return Vector2D(mouseXUnit, mouseYUnit);
}

void MovementHandling::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    double xDiff = xpos - mouseX;
    double yDiff = ypos - mouseY;
    mouseX = xpos;
    mouseY = ypos;
    if (GlobalStates::controlCamera) {
        (*GlobalStates::renderWindow).cam.theta -= xDiff / 200;
        (*GlobalStates::renderWindow).cam.phi += yDiff / 200;
    }
    if (GlobalStates::dragVertex) {
        //process: select a single vector at the beginning (do in mouse_callback) and stick with it during a given drag sequence
        // select vector by determining the closest 2d vector to the point (should not be too difficult)
        // calculate vector projection of vector going from the original (cached) position of the directional indicator to the mouse position onto the selected directional indicator vector
       
        Vector2D mousePos = getMouseUnit(window);

        Vector2D cachedDirectionalIndicator = (*GlobalStates::renderWindow).testEditor.cachedDirectionalIndicatorPos;
        Vector2D cachedInitialProjection = (*GlobalStates::renderWindow).testEditor.cachedInitialProjection;
        Vector2D relativeMousePos = Vector2D(mousePos.x - cachedDirectionalIndicator.x, mousePos.y - cachedDirectionalIndicator.y);
        Vector2D projectedVector = visualTransform::projectVector(relativeMousePos, cachedInitialProjection);

        double angleDifference = visualTransform::angleBetweenVectors(projectedVector, cachedInitialProjection); // used to check if vectors are in same direction

        double currentProjectionLength = projectedVector.Magnitude();
        if (!(angleDifference < 0.1 && angleDifference > -0.1)) currentProjectionLength = -currentProjectionLength;
        
        double originalProjectionLength = cachedInitialProjection.Magnitude();
        double projectionDifferential = currentProjectionLength - originalProjectionLength;
        double quarterUnitProjection = (*GlobalStates::renderWindow).testEditor.cachedDirectionalVector.Magnitude(); // gets the original directional vector used for projection -> known to be 0.25 units
        double newOffsetAmount = .25 * (projectionDifferential / quarterUnitProjection);

        int dragDirection = (*GlobalStates::renderWindow).testEditor.dragDirection;
        int highlightedVertex = (*GlobalStates::renderWindow).testEditor.highlightedVertex;
        Vector3D currentPointPosition = (*GlobalStates::renderWindow).testEditor.currentModel.getVertex(highlightedVertex);
        Vector3D cached3DDirectionalIndicator = (*GlobalStates::renderWindow).testEditor.cached3DDirectionalIndicatorPos;

        

        if (dragDirection == 0) {
            if (GlobalStates::snapVertex) {
                double overage = fmod(cached3DDirectionalIndicator.x + newOffsetAmount, 0.2); // 0.2 manually set based on current snapgrid 0.2 distance.  probably want to create object for each for modularity
                newOffsetAmount -= overage;
            }
            (*GlobalStates::renderWindow).testEditor.currentModel.setVertex(highlightedVertex, Vector3D(cached3DDirectionalIndicator.x + newOffsetAmount, currentPointPosition.y, currentPointPosition.z));
        }
        else if (dragDirection == 1) {
            if (GlobalStates::snapVertex) {
                double overage = fmod(cached3DDirectionalIndicator.y + newOffsetAmount, 0.2); // 0.2 manually set based on current snapgrid 0.2 distance.  probably want to create object for each for modularity
                newOffsetAmount -= overage;
            }
            (*GlobalStates::renderWindow).testEditor.currentModel.setVertex(highlightedVertex, Vector3D(currentPointPosition.x, cached3DDirectionalIndicator.y + newOffsetAmount, currentPointPosition.z));
        }
        else if (dragDirection == 2) {
            if (GlobalStates::snapVertex) {
                double overage = fmod(cached3DDirectionalIndicator.z + newOffsetAmount, 0.2); // 0.2 manually set based on current snapgrid 0.2 distance.  probably want to create object for each for modularity
                newOffsetAmount -= overage;
            }
            (*GlobalStates::renderWindow).testEditor.currentModel.setVertex(highlightedVertex, Vector3D(currentPointPosition.x, currentPointPosition.y, cached3DDirectionalIndicator.z + newOffsetAmount));
        }
    }
}

void MovementHandling::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            GlobalStates::controlCamera = true;
        }
        else if (action == GLFW_RELEASE) {
            GlobalStates::controlCamera = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if ((*GlobalStates::renderWindow).testEditor.highlightedVertex != -1) {
                GlobalStates::dragVertex = true;

                Vector2D mousePos = getMouseUnit(window);
                (*GlobalStates::renderWindow).testEditor.calculateDragDirection(mousePos);
            }
            else {
                Vector2D mousePos = getMouseUnit(window);
                (*GlobalStates::renderWindow).testEditor.highlightVertex(mousePos);
            }
        }
        else if (action == GLFW_RELEASE) {
            if ((*GlobalStates::renderWindow).testEditor.highlightedVertex != -1) {
                GlobalStates::dragVertex = false;
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).testEditor.unHighlightVertex();
        }
    }
}

void MovementHandling::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_A) { // rewrite this entire section to use events, referencing global states and allowing renderwindow to derive from event
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveLeft = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveLeft = false;
        }
    }
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveForward = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveForward = false;
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveBackward = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveBackward = false;
        }
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveRight = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveRight = false;
        }
    }
    if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.lookLeft = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.lookLeft = false;
        }
    }
    if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.lookRight = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.lookRight = false;
        }
    }
    if (key == GLFW_KEY_DOWN) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.lookDown = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.lookDown = false;
        }
    }
    if (key == GLFW_KEY_UP) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.lookUp = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.lookUp = false;
        }
    }
    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveUp = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveUp = false;
        }
    }
    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            (*GlobalStates::renderWindow).cam.moveDown = true;
        }
        else if (action == GLFW_RELEASE) {
            (*GlobalStates::renderWindow).cam.moveDown = false;
        }
    }
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.addVisualVertex();
            }
        }
        if (key == GLFW_KEY_K) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(0, -0.01, 0));
            }
        }
        if (key == GLFW_KEY_J) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(-.01, 0, 0));
            }
        }
        if (key == GLFW_KEY_L) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(0.01, 0, 0));
            }
        }
        if (key == GLFW_KEY_I) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(0, 0.01, 0));
            }
        }
        if (key == GLFW_KEY_U) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(0, 0, 0.01));
            }
        }
        if (key == GLFW_KEY_O) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.modifyVisualVertex(Vector3D(0, 0, -0.01));
            }
        }
        if (key == GLFW_KEY_DELETE) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.removeVisualVertex();
            }
        }
        if (key == GLFW_KEY_H) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.collapseThreeVertices();
            }
        }
        if (key == GLFW_KEY_G) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.collapseFourVertices();
            }
        }
        if (key == GLFW_KEY_V) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.connectVertex(0);
            }
        }
        if (key == GLFW_KEY_B) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.connectVertex(1);
            }
        }
        if (key == GLFW_KEY_N) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.collapseConnectedVertex(false);
            }
        }
        if (key == GLFW_KEY_M) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.collapseConnectedVertex(true);
            }
        }
        if (key == GLFW_KEY_C) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.connectExistingVertices();
            }
        }
        if (key == GLFW_KEY_X) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.createSnapVertex();
            }
        }
        if (key == GLFW_KEY_T) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.snapZ++;
            }
        }
        if (key == GLFW_KEY_Y) {
            if (GlobalStates::placementMode) {
                (*GlobalStates::renderWindow).testEditor.snapZ--;
            }
        }
        if (key == GLFW_KEY_SLASH) {
            handleCommand();
        }
        if (key == GLFW_KEY_LEFT_CONTROL) {
            GlobalStates::snapVertex = !GlobalStates::snapVertex;
        }
    }
}