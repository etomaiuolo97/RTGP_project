#version 410

layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec2 a_TextureCoords;

uniform mat4 u_ModelMatrix;

out vec2 textureCoords;


void main(void){

	gl_Position = u_ModelMatrix * vec4(a_Position, 1.0);

	textureCoords = a_TextureCoords;
}