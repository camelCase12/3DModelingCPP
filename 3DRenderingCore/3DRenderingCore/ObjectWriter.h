#pragma once
#include "CShape.h"
#include <string>

void writeFile(CShape shape, std::string file); // writes an object to a file name in the directory, clearing and overwriting the file
void createFile(std::string fileName);