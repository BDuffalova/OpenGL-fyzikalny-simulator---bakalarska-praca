#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
private:
	GLuint shaderID, uniformColor, uniformModel;

	void CompmileShader(const char* vertexCode, const char* fragmentCode);

	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);

	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocaton);

	GLuint GetColorLocation();

	GLuint GetModelLocation();

	GLuint GetShaderID();

	void UseShader();

	void ClearShader();

	~Shader();

};