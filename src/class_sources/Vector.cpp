#include "..\class_headers\Vector.h"
#include <iostream>



Vector::Vector(float vx, float vy) {
	float v = sqrt((pow(vx, 2) + pow(vy, 2)));
	float points[] = { 0.f, 0.f, 0.f,  v, 0.f, 0.f, v - 3.f, 0.5f, 0.f, v - 3.f, -0.5f, 0.f };
	this->vertices = new float[12];
	for (int i = 0; i < 12; i++) {
		vertices[i] = points[i];
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
