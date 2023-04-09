#pragma once
#include "Settings.h"

class Vector {
private:
	float* vertices;


public:
	Vector(float vx, float vy);
	~Vector();
	float getX();
	float getY();
	float* getVertices();
};