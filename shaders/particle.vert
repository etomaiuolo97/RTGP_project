#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

out vec4 worldPosition;
out vec3 worldNormal;

void main()
{
	worldPosition = u_Transform * vec4(a_Position, 1.0);
	worldNormal = mat3(transpose(inverse(u_Transform))) * a_Normal;

	gl_Position = u_Projection * u_View * worldPosition;
}