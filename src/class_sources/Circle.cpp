#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../class_headers/Circle.h"


Circle::Circle() {
    this->vx = 0;
    this->vy = 0;
    this->x = 0;
    this->y = 0;

}

Circle::~Circle() {
    std::cout << "Object destructed" << std::endl;
}

void Circle::initCircle(){

    float theta = 3.1415926 * 2 / float(NUM_SEGMENTS);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = 0.2;//we start at angle = 0 

    float y = 0;

    vertices[0] = 0.f;
    vertices[1] = 0.f;
    vertices[2] = 0.f;

    for (int ii = 3; ii <= NUM_VERTICES; ii += 3)
    {
        vertices[ii] = x;
        vertices[ii + 1] = y;
        vertices[ii + 2] = 0.f;

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -y;
        float ty = x;

        //add the tangential vector 

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 

        x *= radial_factor;
        y *= radial_factor;

    }

}

float* Circle::getVertices()
{
    return vertices;
}
