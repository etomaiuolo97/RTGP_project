// Same functionalities as water.frag but with some parameter changed and with no depth map

#version 410 core

out vec4 o_Color;

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;
in vec3 normal;

uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;
uniform sampler2D u_DuDvMap;
uniform sampler2D u_NormalMap;
uniform vec3 u_LightColor;

uniform float u_MoveFactor;

const float waveStrength = 0.02f;
const float shineDamper = 20.0f;
const float reflectivity = 0.3f;

void main(void) {

	// To the water coordinates system
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, ndc.y);

	// DuDv map distortion
	vec2 repeatedTexCoords = mod(textureCoords, 1.0f);
	vec2 distortedTexCoords = texture(u_DuDvMap, vec2(repeatedTexCoords.x + u_MoveFactor, repeatedTexCoords.y)).rg * 0.1;
	distortedTexCoords = repeatedTexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + u_MoveFactor);
	vec2 totalDistortion = (texture(u_DuDvMap, distortedTexCoords).rg * 2.0f - 1.0f) * waveStrength;
	
	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords = clamp(reflectionTexCoords, 0.001, 0.999);

	vec4 reflectColor = texture(u_ReflectionTexture, reflectionTexCoords);
	vec4 refractColor = texture(u_RefractionTexture, refractionTexCoords);

	// Fresnel effect
	vec4 normalMapColor = texture(u_NormalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
	
	vec3 viewVector = normalize(toCameraVector);
	// - multiply the refractive factor to make the water more transparent
	float refractiveFactor = dot(viewVector, normal) * 2.0;
	refractiveFactor = pow(refractiveFactor, 0.5f);

	// Light reflection
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	// - multiply the specular factor to make the water less specular
	float specular = max(dot(reflectedLight, viewVector), 0.0f) * 0.8;
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = u_LightColor * specular * reflectivity;

	o_Color = mix(reflectColor, refractColor, refractiveFactor);
	o_Color += vec4(specularHighlights, 0.0f);
}