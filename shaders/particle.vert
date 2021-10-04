#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

out vec3 worldNormal;
out vec4 worldPosition;
out vec4 clipSpace;

void main()
{
	worldPosition = u_Transform * vec4(a_Position, 1.0f);
    clipSpace = u_Projection * u_View * worldPosition;

    worldNormal = mat3(transpose(inverse(u_Transform))) * a_Normal;

    gl_Position = clipSpace;

}