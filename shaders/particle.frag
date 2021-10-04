#version 410 core

layout (location = 0) out vec4 o_Color;

in vec3 worldNormal;
in vec4 worldPosition;
in vec4 clipSpace;

uniform sampler2D u_ReflectionTexture;
uniform sampler2D u_RefractionTexture;
uniform sampler2D u_DepthMap;
uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

const float eta = 0.4f;
const float fresnelPower = 3.0f;

const float shineDamper = 10.0f;
const float reflectivity = 0.2f;

void main()
{
	vec3 N = normalize(worldNormal);

    // Vector from vertex to camera in world coordinates
    vec3 V = normalize(worldPosition.xyz - u_CameraPosition);

    // Incidence light direction in world coordinates
    vec3 L = normalize(worldPosition.xyz - u_LightPosition);

    // Half vector in world coordinates
    vec3 H = normalize(L + V);

    // Reflection vector of V with respect to normal
    vec3 R = reflect(V, N);
 
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	float near = 0.1f;
	float far = 10000.0f;
	float depth = texture(u_DepthMap, vec2(ndc.x, ndc.y)).r;
	float floorDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

    depth = gl_FragCoord.z;
	float waterDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));
	float waterDepth = floorDistance - waterDistance;

	float refractiveFactor = dot(V, N);
	refractiveFactor = pow(refractiveFactor, 0.8f);

	// Light reflection
	vec3 reflectedLight = reflect(-L, N);
	float specular = max(dot(reflectedLight, V), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = vec3(1.0f) * specular * reflectivity * clamp(depth / 1.0, 0.2f, 0.8f);

    // we sample the texture cube using the components of the reflection vector as texture coordinates.
    vec4 refractedColor = texture(u_RefractionTexture, vec2(ndc.x, ndc.y));
    vec4 reflectedColor = texture(u_ReflectionTexture, vec2(ndc.x, ndc.y));

    // Fresnel equation with Schlik's approximation
    // F(0°) factor
    float F0 = ((1.0 - eta) * (1.0 - eta)) / ((1.0 + eta) * (1.0 + eta));
    // ratio between reflection and refraction
    float Ratio = F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0.0), fresnelPower);

    // we merge the 2 colors, using the ratio calculated with the Fresnel equation
    
    o_Color = mix(refractedColor, reflectedColor, clamp(Ratio, 0.0, 1.0));
    // o_Color = mix(refractedColor, reflectedColor, refractiveFactor);
    o_Color += vec4(specularHighlights, 0.2f);
    o_Color.a = clamp(depth / 1.0, 0.5, 1.0f);
}