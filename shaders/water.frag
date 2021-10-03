#version 410 core

out vec4 o_Color;

in vec4 clipSpace;

uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;

void main(void) {
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);

	vec4 reflectColor = texture(u_ReflectionTexture, reflectionTexCoords);
	vec4 refractColor = texture(u_RefractionTexture, refractionTexCoords);

	o_Color = mix(reflectColor, refractColor, 0.5);
}