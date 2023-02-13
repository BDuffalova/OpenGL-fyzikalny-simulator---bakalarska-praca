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


    //vstupy sú poèet polí ktoré chceme vytvori a referenciu na premennú do ktorej chceme uchova ID 
    //definuje priestor v pamäti, kde sa vytvorí vertex array a vráti jej id
    glGenVertexArrays(1, VAO);

    glBindVertexArray(*VAO);

    //podobne ako v gen vertex array  tak tu sa použije funkcia gen buffers na vytvorenie priestoru v pamäti, vráti sa id
    glGenBuffers(1, VBO);
    //berie taget ku ktorému chceme naviaza buffer a buffer ktorý chcemem nabindova
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    //prvé je target, druhé je ve¾kos obsahu ktorý budeme vklada, tretie sú dáta ktoré chceme použi, static draw znamená, že hodnoty
    //ktoré budeme vklada sa nebudú meni, ak sa hodnoty budú meni tak sa použije dynamic draw
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*lokácia atribútu ktorý chceme kresli, poèet dimenzií/hodnôt ktoré sa spracujú naraz, typ hodnoty ktorá vstupuje do súradníc vrchola
    štvrtá hodnota znaèí, èi sa majú hodnoty normalizova, piata koo¾ko hodnôt sa má preskoèi, posledný je offfset*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
