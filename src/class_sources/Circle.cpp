#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../class_headers/Circle.h"
#define NUM_SEGMENTS 36

Circle::Circle() {
    this->vx = 0;
    this->vy = 0;

}

Circle::~Circle() {
    std::cout << "Object destructed" << std::endl;
}

void Circle::initCircle(GLuint *VAO, GLuint *VBO){

    GLfloat vertices[(NUM_SEGMENTS + 2) * 3] = {};
    float theta = 3.1415926 * 2 / float(NUM_SEGMENTS);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = 0.2;//we start at angle = 0 

    float y = 0;

    vertices[0] = 0.f;
    vertices[1] = 0.f;
    vertices[2] = 0.f;

    for (int ii = 3; ii <= (NUM_SEGMENTS + 1) * 3; ii += 3)
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


    //vstupy s� po�et pol� ktor� chceme vytvori� a referenciu na premenn� do ktorej chceme uchova� ID 
    //definuje priestor v pam�ti, kde sa vytvor� vertex array a vr�ti jej id
    glGenVertexArrays(1, VAO);

    glBindVertexArray(*VAO);

    //podobne ako v gen vertex array  tak tu sa pou�ije funkcia gen buffers na vytvorenie priestoru v pam�ti, vr�ti sa id
    glGenBuffers(1, VBO);
    //berie taget ku ktor�mu chceme naviaza� buffer a buffer ktor� chcemem nabindova�
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //prv� je target, druh� je ve�kos� obsahu ktor� budeme vklada�, tretie s� d�ta ktor� chceme pou�i�, static draw znamen�, �e hodnoty
    //ktor� budeme vklada� sa nebud� meni�, ak sa hodnoty bud� meni� tak sa pou�ije dynamic draw
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*lok�cia atrib�tu ktor� chceme kresli�, po�et dimenzi�/hodn�t ktor� sa spracuj� naraz, typ hodnoty ktor� vstupuje do s�radn�c vrchola
    �tvrt� hodnota zna��, �i sa maj� hodnoty normalizova�, piata koo�ko hodn�t sa m� presko�i�, posledn� je offfset*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
