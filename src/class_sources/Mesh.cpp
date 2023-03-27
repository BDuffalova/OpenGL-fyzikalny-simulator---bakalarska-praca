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

    //vstupy s� po�et pol� ktor� chceme vytvori� a referenciu na premenn� do ktorej chceme uchova� ID 
    //definuje priestor v pam�ti, kde sa vytvor� vertex array a vr�ti jej id
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //podobne ako v gen vertex array  tak tu sa pou�ije funkcia gen buffers na vytvorenie priestoru v pam�ti, vr�ti sa id
    glGenBuffers(1, &VBO);
    //berie taget ku ktor�mu chceme naviaza� buffer a buffer ktor� chcemem nabindova�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //prv� je target, druh� je ve�kos� obsahu ktor� budeme vklada�, tretie s� d�ta ktor� chceme pou�i�, static draw znamen�, �e hodnoty
    //ktor� budeme vklada� sa nebud� meni�, ak sa hodnoty bud� meni� tak sa pou�ije dynamic draw
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    /*lok�cia atrib�tu ktor� chceme kresli�, po�et dimenzi�/hodn�t ktor� sa spracuj� naraz, typ hodnoty ktor� vstupuje do s�radn�c vrchola
    �tvrt� hodnota zna��, �i sa maj� hodnoty normalizova�, piata koo�ko hodn�t sa m� presko�i�, posledn� je offfset*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::RenderMeshCircle(GLuint shaderProgram, float * color)
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

void Mesh::RenderMeshTable(GLuint shaderProgram, float* color)
{

    glBindVertexArray(VAO);
    //glUniform1f(uniformMove , triOffSet); //get location of uniform value of xmove and set it to triOffset
    glUniform4fv(glGetUniformLocation(shaderProgram, "unifColor"), 1, color);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void Mesh::RenderMeshVector(GLuint shaderProgram, float* color)
{
    glBindVertexArray(VAO);
    //glUniform1f(uniformMove , triOffSet); //get location of uniform value of xmove and set it to triOffset
    glUniform4fv(glGetUniformLocation(shaderProgram, "unifColor"), 1, color);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
    glBindVertexArray(0);

}



Mesh::~Mesh()
{
    ClearMesh();
}