#version 330 core

uniform sampler2D fTexture;

in vec4 vColor;
in vec2 TexCoords;
out vec4 FragColor;

void main()
{
	FragColor = texture(fTexture, TexCoords) * vColor;
}
