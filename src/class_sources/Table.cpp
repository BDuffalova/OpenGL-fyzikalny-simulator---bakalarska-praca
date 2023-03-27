#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../class_headers/Table.h"
#include "../class_headers/Settings.h"

Table::Table() {
    std::cout << "Object created" << std::endl;
	float tmp[] = {
		TABLE_OFFSET_LEFT, TABLE_OFFSET_LEFT, 0.f,
		TABLE_OFFSET_LEFT + WIDTH_TABLE, TABLE_OFFSET_LEFT, 0.f,
		TABLE_OFFSET_LEFT + WIDTH_TABLE, TABLE_OFFSET_LEFT + HEIGHT_TABLE, 0.f,
		TABLE_OFFSET_LEFT, TABLE_OFFSET_LEFT + HEIGHT_TABLE, 0.f
	};
    this->vertices = new float[NUM_VERTICES_TABLE];
	for (int i = 0; i < 12; i++) {
		vertices[i] = tmp[i];
		std::cout << vertices[i] << std::endl;
	}
}

Table::~Table() {
    std::cout << "Object destructed" << std::endl;
}

float* Table::getVertices()
{
    return vertices;
}