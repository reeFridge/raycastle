#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

#include "./utils/shader.h"

// Window

void size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow(unsigned int w, unsigned int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* win = glfwCreateWindow(w, h, "raycast", NULL, NULL);
	if (win == NULL)
	{
		return NULL;
	}
	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, size_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return win;
}

// Input

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Data

void initVAO(unsigned int &VAO, unsigned int &VBO)
{
	float vertices[4] = {
		0.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void drawLine(unsigned int &VAO, unsigned int &program, float x0, float y0, float x1, float y1)
{
	glUseProgram(program);
	
	glm::mat4 model(1.0f);
	glm::vec2 position(x0, y0);
	glm::vec2 size(x1 - x0, y1 - y0);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}

int main()
{
	unsigned int w = 800;
	unsigned int h = 600;

	GLFWwindow* win = createWindow(w, h);
	if (win == NULL)
	{
		printf("Error on window creation\n");
		return -1;
	}
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Error on GLAD loading\n");
		return -1;
	}

	unsigned int vs = shader::loadFromFile("./shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fs = shader::loadFromFile("./shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	unsigned int program = shader::createProgram(vs, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f);

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int VAO, VBO;
	initVAO(VAO, VBO);

	float clearColor[4] = {0.2f, 0.2f, 0.2f, 1.0f};

	while (!glfwWindowShouldClose(win))
	{
		process_input(win);
		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);

		float x0 = 400.0f;
		float y0 = 300.0f;
		float x1 = xpos;
		float y1 = ypos;
		drawLine(VAO, program, x0, y0, x1, y1);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// termination
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);

	glfwTerminate();

	return 0;
}
