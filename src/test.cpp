#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "class_headers/Circle.h"
#include "class_headers/Table.h"
#include "class_headers/Mesh.h"
#include "class_headers/Shader.h"




//const GLint WIDTH = 800, HEIGHT = 600;
std::vector<Mesh*> meshList;
std::vector<Circle*> circleList;
GLuint uniformMove = 0;
Shader* shader;
bool direction = true; //true = right, false = left
float triOffSet = 3.0f; //offset of drawn triangle
float triMaxOffSet = 7.0f; //max offset of triangle in scene (-0.7.0.7)
float triIncrement = 0.0005f; //increment of position in each step
//GLfloat * vertices;

float colors[][4] = { {0.48f, 0.156f, 0.41f, 1.f},{0.784f, 0.36f, 0.556f, 1.f}, {0.7f, 0.89f, 1.f, 1.f} };




static const char* vertexShader = "src/shader_codes/shader.vert";
static const char* fragmentShader = "src/shader_codes/shader_green.frag";

void CreateShaders()
{

	shader = new Shader();

	shader->CreateFromFile(vertexShader, fragmentShader);
}

int main(void)
{
	double elapsed_time{ 0 };
	clock_t start_time;
	clock_t end_time;
	GLFWwindow* window;

	Circle* circle1 = new Circle(30.f, 30.f, 15.f, 15.f);
	circle1->initCircle();

	Circle* circle2 = new Circle(70.f, 70.f, -10.f, -10.f);
	circle2->initCircle();

	circleList.push_back(circle1);
	circleList.push_back(circle2);


	Table table;

	Mesh* objCircle1 = new Mesh();
	Mesh* objCircle2 = new Mesh();
	Mesh* objTable = new Mesh();

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
	window = glfwCreateWindow(480, 480, "Hello World", NULL, NULL);
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
	objCircle1->CreateMesh(circle1->getVertices(), NUM_VERTICES_CIRCLE);
	objCircle2->CreateMesh(circle2->getVertices(), NUM_VERTICES_CIRCLE);
	objTable->CreateMesh(table.getVertices(), NUM_VERTICES_TABLE);
	meshList.push_back(objCircle1);
	meshList.push_back(objCircle2);
	meshList.push_back(objTable);
	CreateShaders();
	/*CreateTriangle();*/
	/*CompileShaders(shader2, fShader2);*/
	glm::mat4 projection(glm::ortho(0.f, WIDTH_SCENE, 0.f, HEIGHT_SCENE, POINT_NEAR, POINT_FAR));
	while (!glfwWindowShouldClose(window))
	{
		start_time = clock();
		/* Poll for and process events, get + handle user inputs*/
		glfwPollEvents();
		//CALCULATIONS
/*		circle1->x += elapsed_time * circle1->vx;
		circle1->y += elapsed_time * circle1->vy;

		circle2->x += elapsed_time * circle2->vx;
		circle2->y += elapsed_time * circle2->vy;*/
		for (auto &circle : circleList)
		{
			circle->x += elapsed_time * circle->vx;
			circle->y += elapsed_time * circle->vy;
		}
		//
		glClearColor(1.f, 0.73f, 0.73f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->UseShader();

		uniformMove = shader->GetModelLocation();
		glm::mat4 model, transform;


		glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(projection)); //FALSE to transpose -> no fliping along the diagonal axis
		meshList[2]->RenderMeshTable(shader->GetShaderID(), colors[2]);

		model = glm::mat4();
		model = glm::translate(glm::mat4(1.0f), glm::vec3(circle1->x, circle1->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(15.f, 15.f, 0.0f));
		transform = projection * model;

		glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis

		meshList[0]->RenderMeshCircle(shader->GetShaderID(), colors[0]);
		glUseProgram(0);

		shader->UseShader();
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(circle2->x, circle2->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(15.f, 15.f, 0.0f));
		transform = projection * model;
		glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis

		meshList[1]->RenderMeshCircle(shader->GetShaderID(), colors[1]);

		glUseProgram(0);

		///* Swap front and back buffers */
		glfwSwapBuffers(window);
		end_time = clock();
		elapsed_time = ((double)end_time - (double)start_time) / CLOCKS_PER_SEC;

	}

	glfwTerminate();
	delete shader;
	delete circle1;
	delete circle2;
	delete objCircle1;
	delete objCircle2;
	delete objTable;
	exit(0);
	return 0;
}