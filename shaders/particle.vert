#version 410 core

layout (location = 0) in vec2 position;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//uniform vec2 texOffset1;
//uniform vec2 texOffset2;
uniform vec2 texOffsets;
//uniform vec2 textCoordInfo;
uniform float blendFactor;
uniform float numberRows;


// used to instanced more particles
//uniform mat4 allModelViewMatrices[10000];

void main()
{
	//mat4 modelViewMatrix = allModelViewMatrices[gl_InstanceID];

	vec2 textureCoords = position + vec2(0.5,0.5);
	textureCoords.y = 1.0 - textureCoords.y; 
	//textureCoords /= textureCoords.x;
	textureCoords /= numberRows;

	textureCoords1 = textureCoords + texOffsets.xy;
	textureCoords2 = textureCoords + texOffsets.zw;
	//blend = textCoordInfo.y;
	blend = blendFactor;

	gl_Position = viewMatrix * projectionMatrix * vec4(position, 1.0);
}
