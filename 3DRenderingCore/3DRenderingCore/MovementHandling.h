#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vector2D.h"

class MovementHandling {
public:
	static double mouseX;
	static double mouseY;

	static Vector2D getMouseUnit(GLFWwindow* window);

	static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};