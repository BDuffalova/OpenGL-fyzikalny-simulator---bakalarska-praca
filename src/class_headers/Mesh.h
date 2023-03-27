#pragma once
#include <GL/glew.h>
class Mesh
{
public:

	Mesh();

	void CreateMesh(GLfloat *vertices,unsigned int numOfVertices);
	void RenderMeshCircle(GLuint shaderProgram, float* color);
	void RenderMeshTable(GLuint shaderProgram, float* color);
	void RenderMeshVector(GLuint shaderProgram, float* color);
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO;
	GLsizei indexCount;
};