#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main () {
    worldPosition = modelMatrix * vec4(position, 1.0);
    worldNormal = mat3(transpose(inverse(modelMatrix))) * normal;

    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}