#include "../class_headers/Mesh.h"
#include "../class_headers/Circle.h"
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int numOfVertices)
{
	indexCount = numOfVertices;

    //vstupy sú poèet polí ktoré chceme vytvori a referenciu na premennú do ktorej chceme uchova ID 
    //definuje priestor v pamäti, kde sa vytvorí vertex array a vráti jej id
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //podobne ako v gen vertex array  tak tu sa pouije funkcia gen buffers na vytvorenie priestoru v pamäti, vráti sa id
    glGenBuffers(1, &VBO);
    //berie taget ku ktorému chceme naviaza buffer a buffer ktorı chcemem nabindova
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //prvé je target, druhé je ve¾kos obsahu ktorı budeme vklada, tretie sú dáta ktoré chceme poui, static draw znamená, e hodnoty
    //ktoré budeme vklada sa nebudú meni, ak sa hodnoty budú meni tak sa pouije dynamic draw
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    /*lokácia atribútu ktorı chceme kresli, poèet dimenzií/hodnôt ktoré sa spracujú naraz, typ hodnoty ktorá vstupuje do súradníc vrchola
    štvrtá hodnota znaèí, èi sa majú hodnoty normalizova, piata koo¾ko hodnôt sa má preskoèi, poslednı je offfset*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::RenderMesh(GLuint shaderProgram, float * color)
{
 
    glBindVertexArray(VAO);
    //glUniform1f(uniformMove , triOffSet); //get location of uniform value of xmove and set it to triOffset
    glUniform4fv(glGetUniformLocation(shaderProgram, "unifColor"), 1, color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS + 2);
    glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    indexCount = 0;
}


Mesh::~Mesh()
{
    ClearMesh();
}