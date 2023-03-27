#pragma once
#include "Settings.h"

class Vector {
private:
	float x;
	float y;
	float* vertices;


public:
	Vector();
	~Vector();
	float getX();
	float getY();
	float* getVertices();
};