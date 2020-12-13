#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexY;

uniform mat4 projection;
uniform mat4 model;
uniform vec4 color;
uniform float texX;

out vec4 vColor;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
	vColor = color;
	TexCoords = vec2(texX, aTexY);
}
