#include "GlobalStates.h"

RenderWindow* GlobalStates::renderWindow = &RenderWindow(); // this piece of code is either a. my stupidity manifest or b. everything wrong with c++ in one statement

bool GlobalStates::controlCamera = false;
bool GlobalStates::frameRate = false;
bool GlobalStates::placementMode = false;
bool GlobalStates::dragVertex = false;
bool GlobalStates::snapVertex = false;