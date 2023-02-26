#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class_headers/Circle.h"
#include "class_headers/Mesh.h"
#include "class_headers/Shader.h"




//const GLint WIDTH = 800, HEIGHT = 600;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
GLuint shader1, shader2, uniformMove = 0;
bool direction = true; //true = right, false = left
float triOffSet = 0.0f; //offset of drawn triangle
float triMaxOffSet = 0.7f; //max offset of triangle in scene (-0.7.0.7)
float triIncrement = 0.0005f; //increment of position in each step
//GLfloat * vertices;

float colors[][4] = { {1.f, 0.f, 0.f, 1.f},{0.f, 1.f, 0.f, 1.f} };




static const char* vertexShader = "src/shader_codes/shader.vert";
static const char* fragmentShaderGreen = "src/shader_codes/shader_green.frag";
static const char* fragmentShaderRed = "src/shader_codes/shader_red.frag";

void CreateShaders()
{

    Shader* greenCircleShader = new Shader();

    greenCircleShader->CreateFromFile(vertexShader, fragmentShaderGreen);

    shaderList.push_back(*greenCircleShader);

}

int main(void)
{
    GLFWwindow* window;

    Circle circle1;
    circle1.initCircle();

    Circle circle2;
    circle2.initCircle();

    Mesh *objCircle1 = new Mesh();
    Mesh* objCircle2 = new Mesh();

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
    objCircle1->CreateMesh(circle1.getVertices(), NUM_VERTICES);
    objCircle2->CreateMesh(circle2.getVertices(), NUM_VERTICES);
    meshList.push_back(objCircle1);
    meshList.push_back(objCircle2);
    CreateShaders();
    /*CreateTriangle();*/
    /*CompileShaders(shader2, fShader2);*/
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

        shaderList[0].UseShader();
        uniformMove = shaderList[0].GetModelLocation();
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(triOffSet, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(model)); //FALSE to transpose -> no fliping along the diagonal axis

        meshList[0]->RenderMesh(shaderList[0].GetShaderID(), colors[0]);
        glUseProgram(0);

        shaderList[0].UseShader();
    
        glm::mat4 model2;
        uniformMove = shaderList[0].GetModelLocation();
        model2 = glm::translate(model2, glm::vec3(triOffSet, triOffSet, 0.0f));
        glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(model2)); //FALSE to transpose -> no fliping along the diagonal axis

        meshList[1]->RenderMesh(shaderList[0].GetShaderID(), colors[1]);

        glUseProgram(0);

        ///* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}