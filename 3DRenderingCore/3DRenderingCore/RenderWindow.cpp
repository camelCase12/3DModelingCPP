#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "RenderWindow.h"
#include "ErrorCallback.h"
#include "MovementHandling.h"
#include "CRender.h"
#include "GlobalStates.h"
#include "ModelEditor.h"


void RenderWindow::transformShapes() {
    RenderWindow::testEditor.transform(RenderWindow::cam); // transform test editor
    for (int i = 0; i < RenderWindow::shapes.size(); i++) { // transform all shapes in vector
        shapes.at(i).transform(RenderWindow::cam);
    }
}

void RenderWindow::renderShapes() { // call render function of all objects contained in the render window
    RenderWindow::testEditor.render(); // render test editor
    for (int i = 0; i < RenderWindow::shapes.size(); i++) { // render all shapes in vector
        shapes.at(i).render();
    }
}

void RenderWindow::init() {

    testEditor = ModelEditor(); // initialize model editor with default constructor
    cam = Camera(0, 0, 0, M_PI / 2.01, M_PI / 2); // initialize camera with default parameters

    GlobalStates::renderWindow = this; // sets the global static window reference to the current window being initialized

    glfwSetKeyCallback(window, MovementHandling::key_callback); // hook glfw into my key handling function in movementhandling.cpp
    glfwSetCursorPosCallback(window, MovementHandling::cursor_callback); // hook glfw into my cursor position handling function in movementhandling.cpp
    glfwSetMouseButtonCallback(window, MovementHandling::mouse_callback); // hook glfw into my mouse button handling function in movementhandling.cpp

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    loop(); // Begin rendering loop since everything has initialized
}

void RenderWindow::loop() {

    //double time = glfwGetTime(); // for animations

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        CRender::clear();
        double startTime = glfwGetTime();
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        transformShapes();
        renderShapes();
        //update movement
        cam.applyMovement();
        //update movement

        //PIPELINE

        //PIPELINE


        glBegin(GL_LINES); // drawing lines start
        int size = CRender::lineStartVertices.size();
        for (int i = 0; i < size; i++) {
            Vector3D currentColor = CRender::lineColors.at(i);
            glColor3f(currentColor.x, currentColor.y, currentColor.z);
            Vector2D point1 = CRender::lineStartVertices.at(i);
            Vector2D point2 = CRender::lineEndVertices.at(i);
            glVertex2f(point1.x, point1.y);
            glVertex2f(point2.x, point2.y);
        }
        glColor3f(1, 1, 1);
        glEnd(); // drawing lines end

        glBegin(GL_POINTS); // drawing points start
        size = CRender::points.size();
        for (int i = 0; i < size; i++) {
            glVertex2f(CRender::points.at(i).x, CRender::points.at(i).y);
        }
        glVertex2f(0, 0);
        glEnd();
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        glColor3f(0, 1, 0);
        for (int i = 0; i < CRender::connectedPoints.size(); i++) {
            glVertex2f(CRender::connectedPoints.at(i).x, CRender::connectedPoints.at(i).y);
        }
        glEnd(); // drawing points end

        size = CRender::polygons.size();
        for (int i = 0; i < size; i++) {
            glBegin(GL_POLYGON); // drawing polygon (pseudo-triangle) start
            glColor3f(1, 1, 1);
            for (int j = 0; j < CRender::polygons.at(i).size(); j++) {
                glVertex2f(CRender::polygons.at(i).at(j).x, CRender::polygons.at(i).at(j).y);
            }
            glEnd(); // drawing polygon (pseudo-triangle) end
        }

        glPointSize(1.0f); // reset point size
        glColor3f(1, 1, 1); // reset color
        double frameTime = glfwGetTime() - startTime;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (GlobalStates::frameRate) {
            std::cout << frameTime * 1000 << " ms\n"; // test frame response time
        }
    }

    glfwTerminate();
}

RenderWindow::RenderWindow(int width, int height) {
    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "GLFW failed to initialize.";
        return; // will end program
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "3D Rendering", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create window properly.";
        glfwTerminate();
        return; // will end program
    }

    init(); // after changeable parameters have been set, begin initializing glfw window
}

RenderWindow::RenderWindow() {
    //empty constructor for not make error in static pointer contained in global states
}