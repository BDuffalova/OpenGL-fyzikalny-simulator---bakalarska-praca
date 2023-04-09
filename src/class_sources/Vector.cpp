#include "..\class_headers\Vector.h"
#include <iostream>



Vector::Vector(float vx, float vy) {
	float points[] = { 0.f, 0.f, 0.f,  vx, vy, 0.f};
	this->vertices = new float[NUM_VERTICES_TABLE];
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
