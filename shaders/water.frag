#version 410 core

out vec4 o_Color;

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;
uniform sampler2D u_DuDvMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_DepthMap;
uniform vec3 u_LightColor;

uniform float u_MoveFactor;

const float waveStrength = 0.02f;
const float shineDamper = 20.0f;
const float reflectivity = 0.6f;

void main(void) {

	// To the water coordinates system
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, -ndc.y);

	// WATER DEPTH CALCULATION:
	float near = 0.1f;
	float far = 10000.0f;

	// - taking the red component it gives us the depth information: a value between 0 and 1
	float depth = texture(u_DepthMap, refractionTexCoords).r;
	// - distance from the camera of the fountain under the water
	float floorDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

	// - the z component of gl_FragCoord gives us a depth information: a value between 0 and 1
	depth = gl_FragCoord.z;
	// - distance from the camera of the water
	float waterDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

	// - depth of the water
	float waterDepth = floorDistance - waterDistance;

	// DuDv map distortion
	vec2 distortedTexCoords = texture(u_DuDvMap, vec2(textureCoords.x + u_MoveFactor, textureCoords.y)).rg * 0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + u_MoveFactor);
	vec2 totalDistortion = (texture(u_DuDvMap, distortedTexCoords).rg * 2.0f - 1.0f) * waveStrength;
	
	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(u_ReflectionTexture, reflectionTexCoords);
	vec4 refractColor = texture(u_RefractionTexture, refractionTexCoords);

	// Fresnel effect
	vec4 normalMapColor = texture(u_NormalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b, normalMapColor.g * 2.0f - 1.0f);
	normal = normalize(normal);
	
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, 0.5f);

	// Light reflection
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = u_LightColor * specular * reflectivity * clamp(waterDepth / 3.0, 0.2f, 1.0f);

	o_Color = mix(reflectColor, refractColor, refractiveFactor);
	o_Color = mix(o_Color, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.1f);
	o_Color += vec4(specularHighlights, 0.0f);

	// Making water transparent at the edges:
	o_Color.a = clamp(waterDepth / 3.0, 0.2f, 1.0f);

}