#version 410 core

#define NR_LIGHTS 3

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform vec3 lights[NR_LIGHTS];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat3 normalMatrix;

out vec3 lightDirs[NR_LIGHTS];
out vec3 vNormal;
out vec3 vViewPosition;

void main() {
    vec4 mvPosition = viewMatrix * modelMatrix * vec4(position, 1.0);

    vViewPosition = - mvPosition.xyz;
    vNormal = normalize(normalMatrix * normal);

    for (int i = 0; i < NR_LIGHTS; i++) {
        vec4 lightPos = viewMatrix * vec4(lights[i], 1.0);;
        lightDirs[i] = lightPos.xyz - mvPosition.xyz;
    }

    gl_Position = projectionMatrix * mvPosition;
}