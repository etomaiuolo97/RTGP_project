#version 410 core

layout (location = 0) out vec4 o_Color;

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

uniform sampler2D particleTexture;

void main()
{
	vec4 colour1 = texture(particleTexture, textureCoords1);
	vec4 colour2 = texture(particleTexture, textureCoords2);

	o_Color = mix(colour1, colour2, blend);
}