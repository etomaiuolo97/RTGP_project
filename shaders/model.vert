#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoords;

out vec2 textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

uniform vec3 u_LightPosition;

uniform vec4 u_ClipPlane;

void main (void) {
    vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0f);
    
    gl_ClipDistance[0] = dot(worldPosition, u_ClipPlane);
    
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPosition;

    textureCoords = a_TextureCoords;
    surfaceNormal = (u_ModelMatrix * vec4(a_Normal, 0.0f)).xyz;
    toLightVector = u_LightPosition - worldPosition.xyz;
    toCameraVector = (inverse (u_ViewMatrix) * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz - worldPosition.xyz;
}