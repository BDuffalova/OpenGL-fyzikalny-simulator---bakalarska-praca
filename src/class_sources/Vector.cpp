#include "..\class_headers\Vector.h"
#include <iostream>



Vector::Vector(float vx, float vy) {
	// Calculate the direction of the vector
	float angle = atan2(vx, vy);

	//// Convert to degrees
	angle = angle * 180 / 3.14;
	float x3 = vx - 5 * sin(angle);
	float y3 = vy + 5 * cos(angle);
	float x4 = vx + 5 * sin(angle);
	float y4 = vy - 5 * cos(angle);
	float points[] = { 0.f, 0.f, 0.f,  vx, vy, 0.f, x3, y3, 0.f, x4, y4, 0.f };
	this->vertices = new float[12];
	for (int i = 0; i < 12; i++) {
		vertices[i] = points[i];
		std::cout << vertices[i] << std::endl;
	}

}

Vector::~Vector() {
    std::cout << "Object destructed" << std::endl;
}

float Vector::getX()
{
    return 0.0f;
}

float Vector::getY()
{
    return 0.0f;
}

float* Vector::getVertices()
{
	return vertices;
}
