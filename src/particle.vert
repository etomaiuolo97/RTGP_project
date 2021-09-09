#version 410 core

layout (location = 0) in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	position = viewMatrix * projectionMatrix * vec4(position, 1.0);
}
