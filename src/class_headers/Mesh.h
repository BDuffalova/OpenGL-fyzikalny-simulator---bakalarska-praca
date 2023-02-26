#pragma once
#include <GL/glew.h>
class Mesh
{
public:

	Mesh();

	void CreateMesh(GLfloat *vertices,unsigned int numOfVertices);
	void RenderMesh(GLuint shaderProgram, float* color);
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO;
	GLsizei indexCount;
};