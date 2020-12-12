#version 330 core

layout (location = 0) in vec2 aPos;

uniform mat4 projection;
uniform mat4 model;
uniform vec4 color;

out vec4 vColor;

void main()
{
	gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
	vColor = color;
}
