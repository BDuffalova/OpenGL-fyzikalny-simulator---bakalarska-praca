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

}

Table::~Table() {
    std::cout << "Object destructed" << std::endl;
}

float* Table::getVertices()
{
    return vertices;
}