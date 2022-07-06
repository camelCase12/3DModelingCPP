#pragma once
#include "RenderWindow.h"
class GlobalStates
{
public:
	static RenderWindow* renderWindow; // reference to the main window
	static bool controlCamera; // when mmb pressed -> move camera with mouse
	static bool frameRate; // when frameRate on command issued -> print frame time each frame
	static bool placementMode; // when placementMode on command issued -> change command scheme to model modification
	static bool dragVertex; // when mouse held on directionalindicator arrow -> move point with mouse
	static bool snapVertex; // when ctrl held, cause dragVertex to snap to unit
};