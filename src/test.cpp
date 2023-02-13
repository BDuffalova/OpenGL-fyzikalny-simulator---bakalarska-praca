#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "class_headers/Circle.h"
#define NUM_SEGMENTS 36



//const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO;
GLuint shader, uniformMove;
bool direction = true; //true = right, false = left
float triOffSet = 0.0f; //offset of drawn triangle
float triMaxOffSet = 0.7f; //max offset of triangle in scene (-0.7.0.7)
float triIncrement = 0.0005f; //increment of position in each step
//GLfloat * vertices;


//Vertex shader
static const char* vShader = "                                \n\
#version 330                                                  \n\
                                                              \n\
layout (location = 0) in vec3 pos;                            \n\
                                                              \n\
uniform mat4 model;                                          \n\
                                                              \n\
                                                              \n\
void main()                                                   \n\
{                                                             \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0); \n\
}";

static const char* fShader = "                                \n\
#version 330                                                   \n\
                                                              \n\
out vec4 colour;                                              \n\
                                                              \n\
void main()                                                   \n\
{                                                             \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}";



void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);


}

void CompileShaders()
{
    shader = glCreateProgram();
    if (!shader) {
        printf("Error creating shader program!");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformMove = glGetUniformLocation(shader, "model");
}

int main(void)
{
    GLFWwindow* window;
    Circle circle;

    /* Initialize the library */
    if (!glfwInit())
    {
        printf("GLFW initialisation failed");
        glfwTerminate();
        return 1;
    }




    /*setup GLFW window*/
 /*   openGL version, VERSION 3*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /*how openGL treats the code, won't be backwards compatible = core profile*/
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*will be forward compatible*/
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        printf("GLFW window creation failed");
        glfwTerminate();
        return -1;
    }

    /*get buffer size information*/
    int bufferWidth, bufferHeight;

    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

   /* allow modern extentions features*/
    glewExperimental = GL_TRUE;

   /* must be after valid context*/
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Innit error!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    /*setup viewboard size*/

    glViewport(0, 0, bufferWidth, bufferHeight);

    /*generate buffer*/
    /*unsigned int buffer; //inicializuje premennu buffer
    glGenBuffers(1, &buffer); //vráti id bufferu
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //vyber buffer, array lebo blok pamate
    dalsi krok je specifikacia dat
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);*/

    /* Loop until the user closes the window */

    /*CreateTriangle();*/

    circle.initCircle(&VAO, &VBO);

    CompileShaders();
    while (!glfwWindowShouldClose(window))
    {
        /* Poll for and process events, get + handle user inputs*/
        glfwPollEvents();

        if (direction)
        {
            triOffSet += triIncrement;
        }
        else
        {
            triOffSet -= triIncrement;
        }

        if (abs(triOffSet) >= triMaxOffSet)
        {
            direction = !direction;
        }

        /* Render here */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        glBindVertexArray(VAO);
        glm::mat4 model2;
        model2 = glm::translate(model2, glm::vec3(triOffSet, 0.0f, 0.0f));

        glm::mat4 model;
        model = glm::translate(model, glm::vec3(triOffSet, triOffSet, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.0f));
      


        //glUniform1f(uniformMove , triOffSet); //get location of uniform value of xmove and set it to triOffset
        glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(model2)); //FALSE to transpose -> no fliping along the diagonal axis



        glDrawArrays(GL_TRIANGLE_FAN,0, NUM_SEGMENTS+2);

        glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(model)); //FALSE to transpose -> no fliping along the diagonal axis



        glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS +2);

        glm::mat4 model3;
        model3 = glm::translate(model3, glm::vec3(0.f, 0.f, 0.0f));
        glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(model3)); //FALSE to transpose -> no fliping along the diagonal axis

        glDrawArrays(GL_LINE_LOOP, NUM_SEGMENTS + 2, 2);
        glBindVertexArray(0);

        glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}