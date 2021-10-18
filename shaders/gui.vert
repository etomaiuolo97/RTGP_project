#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec4 worldPosition;
out vec3 worldNormal;

uniform mat4 transformationMatrix;

void main(void){

	worldPosition = transformationMatrix * vec4(position, 1.0);

	worldNormal = mat3(transpose(inverse(transformationMatrix))) * normal;
	
	gl_Position = worldPosition;

}