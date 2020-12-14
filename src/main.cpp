#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

#include "./utils/shader.h"
#include "./utils/texture.h"

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
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
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

void initVAO(unsigned int &VAO, unsigned int &VBO)
{
	float vertices[8] = {
		0.0f, 0.0f,
		0.0f,
		
		1.0f, 1.0f,
		1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

// Utils

void drawLine(unsigned int &VAO, unsigned int &program, float x0, float y0, float x1, float y1, glm::vec4 &color, float texX, unsigned int &texture)
{
	glUseProgram(program);
	
	glm::mat4 model(1.0f);
	glm::vec2 position(x0, y0);
	glm::vec2 size(x1 - x0, y1 - y0);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform4fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1f(glGetUniformLocation(program, "texX"), texX);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}

#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight] =
{
	{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
	{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
	{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
	{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
	{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
	{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
	{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
	{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

// colors

glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);

int main()
{
	int w = 1024;
	int h = 768;

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

	unsigned int textures[8];
	textures[0] = texture::loadTexture("./textures/pics/greystone.png");
	textures[1] = texture::loadTexture("./textures/pics/bluestone.png");
	textures[2] = texture::loadTexture("./textures/pics/colorstone.png");
	textures[3] = texture::loadTexture("./textures/pics/eagle.png");
	textures[4] = texture::loadTexture("./textures/pics/redbrick.png");
	textures[5] = texture::loadTexture("./textures/pics/wood.png");
	textures[6] = texture::loadTexture("./textures/pics/purplestone.png");
	textures[7] = texture::loadTexture("./textures/pics/mossy.png");

	unsigned int vs = shader::loadFromFile("./shaders/vertex.glsl", GL_VERTEX_SHADER);
	unsigned int fs = shader::loadFromFile("./shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	unsigned int program = shader::createProgram(vs, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -1.0f, 1.0f);

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "fTexture"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int VAO, VBO;
	initVAO(VAO, VBO);

	float clearColor[4] = {0.2f, 0.2f, 0.2f, 1.0f};

	// vars
	glm::vec2 position(22.0f, 12.0f);
	glm::vec2 direction(-1.0f, 0.0f);
	glm::vec2 plane(0.0f, 0.66f);
	float lastTime = 0.0f, deltaTime = 0.0f;

	while (!glfwWindowShouldClose(win))
	{
		process_input(win);
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		float moveSpeed = deltaTime * 5.0f;
		float rotateSpeed = deltaTime * 7.0;

		if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
		{
			position += direction * moveSpeed;
		}

		if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			position -= direction * moveSpeed;
		}

		if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			direction = glm::rotate(direction, -glm::radians(10.0f * rotateSpeed));
			plane = glm::rotate(plane, -glm::radians(10.0f * rotateSpeed));;
		}

		if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			direction = glm::rotate(direction, glm::radians(10.0f * rotateSpeed));
			plane = glm::rotate(plane, glm::radians(10.0f * rotateSpeed));;
		}

		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);


		for (int x = 0; x < w; x++)
		{
			float cameraX = 2 * x / float(w) - 1.0f; // on the camera plane: left (x=0) = -1.0, middle (x=w/2) = 0.0, right (x=w) = 1.0
			glm::vec2 rayDirection = direction + plane * cameraX; // rayDirection goes trough all length of camera plane
			
			// which box of the map we're in
			int mapX = int(position.x);
			int mapY = int(position.y);

			// length of ray from current position to next x/y side
			float sideDistX, sideDistY;
			// length of ray from one x/y side to next x/y side (mini hypotenuse)
			float deltaDistX = std::abs(1.0f / rayDirection.x); // constant for current rayDirection
			float deltaDistY = std::abs(1.0f / rayDirection.y); // constant for current rayDirection

			bool hit = false;
			int side = 0; // 0 = x-side, 1 = y-side

			int stepX, stepY;

			if (rayDirection.x < 0.0f)
			{
				stepX = -1;
				sideDistX = (position.x - mapX) * deltaDistX; // perpendicular(x side) * mini hypotenuse(x side) = real delta distance(x side)
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0f - position.x) * deltaDistX;
			}

			if (rayDirection.y < 0.0f)
			{
				stepY = -1;
				sideDistY = (position.y - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0f - position.y) * deltaDistY;
			}

			// Digital Differential Analyzer
			while (!hit)
			{
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}

				if (worldMap[mapX][mapY] > 0) hit = true;
			}

			float perpWallDist;

			if (side == 0)
			{
				perpWallDist = (mapX - position.x + (1 - stepX) / 2) / rayDirection.x;
			}
			else
			{
				perpWallDist = (mapY - position.y + (1 - stepY) / 2) / rayDirection.y;
			}

			int lineHeight = int(h / perpWallDist); // the higher dist to the wall, the less result line height
			int drawStart = -lineHeight / 2 + h / 2; // y-center - half of the line
			if (drawStart < 0) drawStart = 0; // clamping
			int drawEnd = lineHeight / 2 + h / 2; // y-center + half of the line
			if (drawEnd >= h) drawEnd = h - 1; // clamping

			glm::vec4 color = yellow;

			switch (worldMap[mapX][mapY])
			{
				case 1: color = red; break;
				case 2: color = green; break;
				case 3: color = blue; break;
				case 4: color = white; break;
				default: color = yellow; break;
			}

			if (side == 1) { color = color / 2.0f; }

			float wallX; //where exactly the wall was hit
			if (side == 0)
			{
				wallX = position.y + perpWallDist * rayDirection.y;
			}
			else
			{
				wallX = position.x + perpWallDist * rayDirection.x;
			}
			wallX -= floor(wallX);

			drawLine(VAO, program, float(x), float(drawStart), float(x), float(drawEnd), white, wallX, textures[worldMap[mapX][mapY] - 1]);
		}

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
