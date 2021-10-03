#version 410 core

out vec4 o_Color;

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;

uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;
uniform sampler2D u_DuDvMap;

uniform float u_MoveFactor;

const float waveStrength = 0.02;

void main(void) {

	// To the water coordinates system
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);

	// DuDv map distortion
	vec2 distortion1 = (texture(u_DuDvMap, vec2(textureCoords.x + u_MoveFactor, textureCoords.y)).rg * 2.0f - 1.0f) * waveStrength;
	vec2 distortion2 = (texture(u_DuDvMap, vec2(textureCoords.x + u_MoveFactor, textureCoords.y + u_MoveFactor)).rg * 2.0f - 1.0f) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;
	
	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(u_ReflectionTexture, reflectionTexCoords);
	vec4 refractColor = texture(u_RefractionTexture, refractionTexCoords);

	// Fresnel effect
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, vec3(0.0f, 1.0f, 0.0f));
	// refractiveFactor = pow(refractiveFactor, 10.0);

	o_Color = mix(reflectColor, refractColor, refractiveFactor);
}