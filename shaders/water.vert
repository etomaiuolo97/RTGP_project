#version 410 core

layout (location = 0) in vec2 a_Position;

out vec4 clipSpace;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main(void) {

	clipSpace = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Position.x, 0.0, a_Position.y, 1.0);
	
	gl_Position = clipSpace;

}