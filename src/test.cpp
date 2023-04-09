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
#include "class_headers/Vector.h"




//const GLint WIDTH = 800, HEIGHT = 600;
std::vector<Mesh*> meshListCircle;
std::vector<Mesh*> meshListVelocity;
std::vector<Circle*> circleList;
std::vector<Vector*> velocityList;
GLuint uniformMove = 0;
Shader* shader;
bool direction = true; //true = right, false = left
float triOffSet = 3.0f; //offset of drawn triangle
float triMaxOffSet = 7.0f; //max offset of triangle in scene (-0.7.0.7)
float triIncrement = 0.0005f; //increment of position in each step
//GLfloat * vertices;

float colors[][4] = { {0.48f, 0.156f, 0.41f, 1.f},{0.784f, 0.36f, 0.556f, 1.f}, {0.7f, 0.89f, 1.f, 1.f} };
float points[] = { 0.f, 0.f, 0.f,  380.f,380.f, 0.f,  385.f,385.f, 0.f, 5.f, 5.f, 0.f };



static const char* vertexShader = "src/shader_codes/shader.vert";
static const char* fragmentShader = "src/shader_codes/shader_green.frag";

void CreateShaders()
{

	shader = new Shader();

	shader->CreateFromFile(vertexShader, fragmentShader);
}

double timeToCollision(double x1, double y1, double vx1, double vy1, double x2, double y2, double vx2, double vy2, double r)
{
	// Calculate the relative position and velocity of ball 2 with respect to ball 1
	double rx = x2 - x1;
	double ry = y2 - y1;
	double vx = vx2 - vx1;
	double vy = vy2 - vy1;

	// Calculate the coefficients of the quadratic equation
	double a = vx * vx + vy * vy;
	double b = 2 * (vx * rx + vy * ry);
	double c = rx * rx + ry * ry - r * r;

	// Solve the quadratic equation
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		// No collision
		return -1;
	}
	double t = (-b - sqrt(discriminant)) / (2 * a);
	if (t < 0)
	{
		// Collision is in the past
		return -1;
	}
	return t;
}

int main(void)
{
	bool colided = false;
	float scale = 25.f;
	float radius = 0.2f * scale;
	float x_tmp = 0.f;
	float y_tmp = 0.f;
	float x_tmp2 = 0.f;
	float y_tmp2 = 0.f;
	float r12_x, r12_y, v12_x, v12_y, dot_product, r12_length, cos_theta, sin_theta, v12_length;
	double relative_velocity_x = 0;
	double relative_velocity_y = 0;
	double colision_distance = 0;
	double colision_time = 0;
	double elapsed_time{ 0 };
	clock_t start_time;
	clock_t end_time;
	GLFWwindow* window;

	Circle* circle1 = new Circle(30.f, 30.f, 50.f, 28.f);
	circle1->initCircle();

	Circle* circle2 = new Circle(70.f, 70.f, -10.f, -40.f);
	circle2->initCircle();

	circleList.push_back(circle1);
	circleList.push_back(circle2);


	Table* table = new Table();
	Vector* velocityCircle1 = new Vector(circle1->vx,circle1->vy);
	Vector* velocityCircle2 = new Vector(circle2->vx, circle2->vy);

	velocityList.push_back(velocityCircle1);
	velocityList.push_back(velocityCircle2);

	Mesh* objCircle1 = new Mesh();
	Mesh* objCircle2 = new Mesh();
	Mesh* objTable = new Mesh();
	Mesh* velocityCircle1Obj1 = new Mesh();
	Mesh* velocityCircle1Obj2 = new Mesh();

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
	objTable->CreateMesh(table->getVertices(), NUM_VERTICES_TABLE);
	velocityCircle1Obj1->CreateMesh(velocityCircle1->getVertices(), 6);
	velocityCircle1Obj2->CreateMesh(velocityCircle2->getVertices(), 6);
	meshListCircle.push_back(objCircle1);
	meshListCircle.push_back(objCircle2);
	meshListCircle.push_back(objTable);
	meshListVelocity.push_back(velocityCircle1Obj1);
	meshListVelocity.push_back(velocityCircle1Obj2);
	CreateShaders();
	/*CreateTriangle();*/
	/*CompileShaders(shader2, fShader2);*/
	glm::mat4 projection(glm::ortho(0.f, WIDTH_SCENE, 0.f, HEIGHT_SCENE, POINT_NEAR, POINT_FAR));
	int num = 0;
	while (!glfwWindowShouldClose(window))
	{
		start_time = clock();
		/* Poll for and process events, get + handle user inputs*/
		glfwPollEvents();
		glClearColor(1.f, 0.73f, 0.73f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//CALCULATIONS

		
		shader->UseShader();

		uniformMove = shader->GetModelLocation();
		glm::mat4 model, transform;


		glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(projection)); //FALSE to transpose -> no fliping along the diagonal axis
		meshListCircle[2]->RenderMeshTable(shader->GetShaderID(), colors[2]);


		for (int x{ 0 }; x < circleList.size(); x++)
		{
			x_tmp = circleList.at(x)->x + elapsed_time * circleList.at(x)->vx;
			y_tmp = circleList.at(x)->y + elapsed_time * circleList.at(x)->vy;
			if (x_tmp + radius > TABLE_OFFSET_LEFT + WIDTH_TABLE)
			{
				colision_distance = (TABLE_OFFSET_LEFT + WIDTH_TABLE) - (circleList.at(x)->x + radius);
				colision_time = colision_distance / circleList.at(x)->vx;
				colision_time = elapsed_time - colision_time;
				elapsed_time = elapsed_time - colision_time;
				circleList.at(x)->vx *= -1;
				circleList.at(x)->x = (TABLE_OFFSET_LEFT + WIDTH_TABLE - radius) + colision_time * circleList.at(x)->vx;
				x_tmp = circleList.at(x)->x;
				circleList.at(x)->y = y_tmp;
				colided = true;

			}
			if (x_tmp - radius < TABLE_OFFSET_LEFT)
			{
				colision_distance = (circleList.at(x)->x - radius) - TABLE_OFFSET_LEFT;
				colision_time = colision_distance / circleList.at(x)->vx;
				colision_time = elapsed_time - colision_time;
				elapsed_time = elapsed_time - colision_time;
				circleList.at(x)->vx *= -1;
				circleList.at(x)->x = (TABLE_OFFSET_LEFT + radius) + colision_time * circleList.at(x)->vx;
				x_tmp = circleList.at(x)->x;
				circleList.at(x)->y = y_tmp;
				colided = true;

			}
			if (y_tmp + radius > TABLE_OFFSET_BOTTOM + HEIGHT_TABLE)
			{
				colision_distance = (TABLE_OFFSET_BOTTOM + HEIGHT_TABLE) - (circleList.at(x)->y + radius);
				colision_time = colision_distance / circleList.at(x)->vy;
				colision_time = elapsed_time - colision_time;
				elapsed_time = elapsed_time - colision_time;
				circleList.at(x)->vy *= -1;
				circleList.at(x)->y = (TABLE_OFFSET_BOTTOM + HEIGHT_TABLE - radius) + colision_time * circleList.at(x)->vy;
				y_tmp = circleList.at(x)->y;
				circleList.at(x)->x = x_tmp;
				colided = true;

			}
			if (y_tmp - radius < TABLE_OFFSET_BOTTOM)
			{
				colision_distance = (circleList.at(x)->y - radius) - TABLE_OFFSET_BOTTOM;
				colision_time = colision_distance / circleList.at(x)->vy;
				colision_time = elapsed_time - colision_time;
				elapsed_time = elapsed_time - colision_time;
				circleList.at(x)->vy *= -1;
				circleList.at(x)->x = (TABLE_OFFSET_BOTTOM + radius)  + colision_time * circleList.at(x)->vy;
				y_tmp = circleList.at(x)->y;
				circleList.at(x)->x = x_tmp;
				colided = true;

			}
			if (circleList.at(x)->x + radius == TABLE_OFFSET_LEFT + WIDTH_TABLE || circleList.at(x)->x - radius == TABLE_OFFSET_LEFT)
			{
				circleList.at(x)->vx *= -1;
				colided = true;
			}

			if (circleList.at(x)->y + radius == TABLE_OFFSET_BOTTOM + HEIGHT_TABLE || circleList.at(x)->y - radius  == TABLE_OFFSET_BOTTOM)
			{
				circleList.at(x)->vy *= -1;
				colided = true;
			}


			if (!colided) {
				x_tmp = circleList.at(0)->x + elapsed_time * circleList.at(0)->vx;
				y_tmp = circleList.at(0)->y + elapsed_time * circleList.at(0)->vy;

				x_tmp2 = circleList.at(1)->x + elapsed_time * circleList.at(1)->vx;
				y_tmp2 = circleList.at(1)->y + elapsed_time * circleList.at(1)->vy;

				r12_x = circleList.at(1)->x - circleList.at(0)->x;
				r12_y = circleList.at(1)->y - circleList.at(0)->y;

				v12_x = circleList.at(1)->vx - circleList.at(0)->vx;
				v12_y = circleList.at(1)->vy - circleList.at(0)->vy;

				colision_time = timeToCollision(circleList.at(0)->x, circleList.at(0)->y, circleList.at(0)->vx, circleList.at(0)->vy, circleList.at(1)->x, circleList.at(1)->y, circleList.at(1)->vx, circleList.at(1)->vy, 2*radius);
				if (colision_time <= elapsed_time && colision_time > 0) {
					x_tmp = circleList.at(0)->x + colision_time * circleList.at(0)->vx;
					y_tmp = circleList.at(0)->y + colision_time * circleList.at(0)->vy;

					x_tmp2 = circleList.at(1)->x + colision_time * circleList.at(1)->vx;
					y_tmp2 = circleList.at(1)->y + colision_time * circleList.at(1)->vy;

					colision_time = abs(elapsed_time - colision_time);

					circleList.at(0)->vx += ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));
					circleList.at(1)->vx -= ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

					circleList.at(0)->vy += ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));
					circleList.at(1)->vy -= ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

					circleList.at(0)->x = x_tmp + colision_time * circleList.at(0)->vx;
					circleList.at(0)->y = y_tmp + colision_time * circleList.at(0)->vy;

					circleList.at(1)->x = x_tmp2 + colision_time * circleList.at(1)->vx;
					circleList.at(1)->y = y_tmp2 + colision_time * circleList.at(1)->vy;
				}
				else {
					circleList.at(x)->x += elapsed_time * circleList.at(x)->vx;
					circleList.at(x)->y += elapsed_time * circleList.at(x)->vy;
				}

		
			}

			colided = false;

			model = glm::mat4();
			model = glm::translate(glm::mat4(1.0f), glm::vec3(circleList.at(x)->x, circleList.at(x)->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 0.0f));
			transform = projection * model;
			glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis
			meshListCircle[x]->RenderMeshCircle(shader->GetShaderID(), colors[x]);

			float velocityCircle1color[] = { 0.f,0.f,1.f,1.f };
			velocityList.at(x)->getVertices()[3] = circleList.at(x)->vx;
			velocityList.at(x)->getVertices()[4] = circleList.at(x)->vy;
			meshListVelocity[x]->UpdateMesh(velocityList.at(x)->getVertices(), 6);
			model = glm::mat4();
			model = glm::translate(glm::mat4(1.0f), glm::vec3(circleList.at(x)->x, circleList.at(x)->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
			transform = projection * model;
			glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis
			meshListVelocity[x]->RenderMeshVector(shader->GetShaderID(), velocityCircle1color);
		}
		
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


//TOTO TREBA VYSKUSAT PRE VYKRESLOVANIE TROJUHOLNIKA NA VEKTORE

//int main() {
//	// Define the coordinates of the line
//	int x1 = 1, y1 = 2, x2 = 4, y2 = 5;
//
//	// Calculate the direction of the vector
//	double dx = x2 - x1;
//	double dy = y2 - y1;
//	double length = sqrt(dx * dx + dy * dy);
//	double ux = dx / length;
//	double uy = dy / length;
//
//	// Calculate the coordinates of the triangle vertices
//	double triangle_length = length / 4; // adjust this value as needed
//	double tx1 = x2 - uy * triangle_length;
//	double ty1 = y2 + ux * triangle_length;
//	double tx2 = x2 + uy * triangle_length;
//	double ty2 = y2 - ux * triangle_length;
//
//	// Print the coordinates of the triangle vertices
//	cout << "Coordinates of the triangle vertices: (" << x2 << ", " << y2 << "), (" << tx1 << ", " << ty1 << "), (" << tx2 << ", " << ty2 << ")" << endl;
//
//	return 0;
//}
//In this code, I first calculate the direction of the vector by subtracting the coordinates of the first point from the coordinates of the second pointand normalizing the resulting displacement vector.Then I calculate the coordinates of the two points that form the triangle by offsetting from the second point along a direction perpendicular to the vector.
//
//The triangle_length variable controls the size of the triangle.In this example, I've set it to one quarter of the length of the vector, but you can adjust this value as needed to make the triangle larger or smaller.
//
//Note that this code assumes that the vector is pointing from the first point(x1, y1) to the second point(x2, y2).If the vector is pointing in the opposite direction, you'll need to reverse the roles of (x1, y1) and (x2, y2) in the code above.
//





