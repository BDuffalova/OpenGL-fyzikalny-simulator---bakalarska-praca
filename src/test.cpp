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
	float scale = 65.f;
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

	Circle* circle1 = new Circle(30.f, 30.f, 30.f, 28.f);
	circle1->initCircle();

	Circle* circle2 = new Circle(70.f, 70.f, -20.f, -20.f);
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
	int num = 0;
	while (!glfwWindowShouldClose(window))
	{
		start_time = clock();
		/* Poll for and process events, get + handle user inputs*/
		glfwPollEvents();
		glClearColor(1.f, 0.73f, 0.73f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//CALCULATIONS
/*		circle1->x += elapsed_time * circle1->vx;
		circle1->y += elapsed_time * circle1->vy;

		circle2->x += elapsed_time * circle2->vx;
		circle2->y += elapsed_time * circle2->vy;*/
		
		shader->UseShader();

		uniformMove = shader->GetModelLocation();
		glm::mat4 model, transform;


		glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(projection)); //FALSE to transpose -> no fliping along the diagonal axis
		meshList[2]->RenderMeshTable(shader->GetShaderID(), colors[2]);

		//r12_x = circleList.at(1)->x - circleList.at(0)->x;
		//r12_y = circleList.at(1)->y - circleList.at(0)->y;

		//v12_x = circleList.at(1)->vx - circleList.at(0)->vx;
		//v12_y = circleList.at(1)->vy - circleList.at(0)->vy;

		//dot_product = r12_x * v12_x + r12_y * v12_y;

		//// Calculate length of relative position vector
		//r12_length = sqrt(pow(r12_x, 2) + pow(r12_y, 2));

		//// Calculate length of relative velocity vector
		//v12_length = sqrt(pow(v12_x, 2) + pow(v12_y, 2));

		//// Calculate cosine of angle between relative position and velocity vectors
		//cos_theta = dot_product / (r12_length * v12_length);

		//// Calculate sin of angle between relative position and velocity vectors
		//sin_theta = sqrt(1 - pow(cos_theta, 2));

		//colision_distance = sqrt(pow(circleList.at(0)->x - circleList.at(1)->x, 2) + pow(circleList.at(0)->y - circleList.at(1)->y, 2));
		//relative_velocity_x = v12_length * cos_theta * r12_x / r12_length - v12_length * sin_theta * r12_y / r12_length;
		//relative_velocity_y = v12_length * cos_theta * r12_y / r12_length + v12_length * sin_theta * r12_x / r12_length;
		//colision_time = (colision_distance - 2 * radius) / sqrt(pow(relative_velocity_x, 2) + pow(relative_velocity_y, 2));

		/*if (r12_length < 2 * radius) {
		
			x_tmp = circleList.at(0)->x + elapsed_time * circleList.at(0)->vx;
			y_tmp = circleList.at(0)->y + elapsed_time * circleList.at(0)->vy;

			x_tmp2 = circleList.at(1)->x + elapsed_time * circleList.at(1)->vx;
			y_tmp2 = circleList.at(1)->y + elapsed_time * circleList.at(1)->vy;

			colision_distance = 2 * radius - r12_length;
			relative_velocity_x = v12_length * cos_theta * r12_x / r12_length - v12_length * sin_theta * r12_y / r12_length;
			relative_velocity_y = v12_length * cos_theta * r12_y / r12_length + v12_length * sin_theta * r12_x / r12_length;
			colision_time = colision_distance / sqrt(pow(relative_velocity_x, 2) + pow(relative_velocity_y, 2));

			circleList.at(0)->x += colision_time * circleList.at(0)->vx * -1;
			circleList.at(0)->y += colision_time * circleList.at(0)->vy * -1;

			circleList.at(1)->x += colision_time * circleList.at(1)->vx * -1;
			circleList.at(1)->y += colision_time * circleList.at(1)->vy * -1;

			colision_time = abs(elapsed_time - colision_time);
			std::cout << "colision time: " << colision_time << std::endl;
			std::cout << "elapsed time: " << elapsed_time << std::endl;

			circleList.at(0)->vx += ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));
			circleList.at(1)->vx -= ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

			circleList.at(0)->vy += ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));
			circleList.at(1)->vy -= ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

			circleList.at(0)->x += 0.001 * circleList.at(0)->vx;
			circleList.at(0)->y += 0.001 * circleList.at(0)->vy;

			circleList.at(1)->x += 0.001 * circleList.at(1)->vx;
			circleList.at(1)->y += 0.001 * circleList.at(1)->vy;

			colided = true;

		}*/
		
		//if (colision_time <= elapsed_time && colision_time > 0) {
		//	x_tmp = circleList.at(0)->x + colision_time * circleList.at(0)->vx;
		//	y_tmp = circleList.at(0)->y + colision_time * circleList.at(0)->vy;

		//	x_tmp2 = circleList.at(1)->x + colision_time * circleList.at(1)->vx;
		//	y_tmp2 = circleList.at(1)->y + colision_time * circleList.at(1)->vy;

		//	colision_time = abs(elapsed_time - colision_time);

		//	circleList.at(0)->vx += ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x,2) + pow(r12_y,2)));
		//	circleList.at(1)->vx -= ((r12_x * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

		//	circleList.at(0)->vy += ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));
		//	circleList.at(1)->vy -= ((r12_y * (r12_x * v12_x + r12_y * v12_y)) / (pow(r12_x, 2) + pow(r12_y, 2)));

		//	circleList.at(0)->x = x_tmp + colision_time * circleList.at(0)->vx;
		//	circleList.at(0)->y = y_tmp + colision_time * circleList.at(0)->vy;

		//	circleList.at(1)->x = x_tmp2 + colision_time * circleList.at(1)->vx;
		//	circleList.at(1)->y = y_tmp2 + colision_time * circleList.at(1)->vy;
		//	colided = true;

		//}

		for (int x{ 0 }; x < circleList.size(); x++)
		{
			x_tmp = circleList.at(x)->x + elapsed_time * circleList.at(x)->vx;
			y_tmp = circleList.at(x)->y + elapsed_time * circleList.at(x)->vy;
			if (x_tmp + radius > TABLE_OFFSET_LEFT + WIDTH_TABLE)
			{
				colision_distance = (TABLE_OFFSET_LEFT + WIDTH_TABLE) - (circleList.at(x)->x + radius);
				colision_time = colision_distance / circleList.at(x)->vx;
				colision_time = elapsed_time - colision_time;
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

				//dot_product = r12_x * v12_x + r12_y * v12_y;

				//// Calculate length of relative position vector
				//r12_length = sqrt(pow(r12_x, 2) + pow(r12_y, 2));

				//// Calculate length of relative velocity vector
				//v12_length = sqrt(pow(v12_x, 2) + pow(v12_y, 2));

				//// Calculate cosine of angle between relative position and velocity vectors
				//cos_theta = dot_product / (r12_length * v12_length);

				//// Calculate sin of angle between relative position and velocity vectors
				//sin_theta = sqrt(1 - pow(cos_theta, 2));


				//v12_x = circleList.at(1)->vx - circleList.at(0)->vx;
				//v12_y = circleList.at(1)->vy - circleList.at(0)->vy;

				//r12_length = sqrt(pow(r12_x, 2) + pow(r12_y, 2));
				//v12_length = sqrt(pow(v12_x, 2) + pow(v12_y, 2));


				//colision_distance = sqrt(pow(x_tmp - x_tmp2,2) + pow(y_tmp - y_tmp2, 2));
				//relative_velocity_x = v12_length * cos_theta * r12_x / r12_length - v12_length * sin_theta * r12_y / r12_length;
				//relative_velocity_y = v12_length * cos_theta * r12_y / r12_length + v12_length * sin_theta * r12_x / r12_length;
				//colision_time = (colision_distance - 2 * radius) / sqrt(pow(relative_velocity_x, 2) + pow(relative_velocity_y, 2));
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
			meshList[x]->RenderMeshCircle(shader->GetShaderID(), colors[x]);
		}
		//


		//model = glm::mat4();
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(circle1->x, circle1->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(15.f, 15.f, 0.0f));
		//transform = projection * model;

		//glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis

		//meshList[0]->RenderMeshCircle(shader->GetShaderID(), colors[0]);
		glUseProgram(0);

		//shader->UseShader();
		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(circle2->x, circle2->y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(15.f, 15.f, 0.0f));
		//transform = projection * model;
		//glUniformMatrix4fv(uniformMove, 1, GL_FALSE, glm::value_ptr(transform)); //FALSE to transpose -> no fliping along the diagonal axis

		//meshList[1]->RenderMeshCircle(shader->GetShaderID(), colors[1]);

		//glUseProgram(0);

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
