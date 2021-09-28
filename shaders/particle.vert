#version 410 core

layout (location = 0) in vec2 a_Position;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 textureCoordsInfo;

void main()
{
	vec2 textureCoords = a_Position + vec2(0.5, 0.5);
	textureCoords.y = 1.0f - textureCoords.y;
	textureCoords /= textureCoordsInfo.x;

	textureCoords1 = textureCoords + texOffset1;
	textureCoords2 = textureCoords + texOffset2;

	blend = textureCoordsInfo.y;

	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 0.0f, 1.0f);
}