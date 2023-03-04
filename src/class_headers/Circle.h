#include "Settings.h"

class Circle {
public:
	float vx;
	float vy;
	float x;
	float y;
	float* vertices;

public:
	Circle();
	Circle(float x, float y, float vx, float vy);
	~Circle();
	void initCircle();
	float* getVertices();
};
