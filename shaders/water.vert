#version 410 core

layout (location = 0) in vec2 a_Position;

out vec4 clipSpace;
out vec2 textureCoords;
// Vector from the water to the camera
out vec3 toCameraVector;
// Vector from the water to the light
out vec3 fromLightVector;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

const float tiling = 0.8f;

void main(void) {

	vec4 worldPosition = u_ModelMatrix * vec4(a_Position.x, 0.0, a_Position.y, 1.0);
	clipSpace = u_ProjectionMatrix * u_ViewMatrix * worldPosition;
	
	gl_Position = clipSpace;

	textureCoords = vec2(a_Position.x / 2.0 + 0.5, a_Position.y / 2.0 + 0.5) * tiling;
	toCameraVector = u_CameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - u_LightPosition;
}