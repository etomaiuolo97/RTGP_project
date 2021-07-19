#version 410 core

const float PI = 3.14159265359;

out vec4 colorFrag;

// Vertex position and normal in world coordinates
in vec4 worldPosition;
in vec3 worldNormal;

// Texture sampler
uniform samplerCube tCube;

uniform vec3 cameraPosition;

uniform vec3 pointLightPosition;

// Eta = ratio between the two refraction indices
uniform float Eta;

// Exponent of Fresnel equation
uniform float mFresnelPower;

// The "type" of subroutine
subroutine vec4 refl_refra();

// Subroutine Uniform
subroutine uniform refl_refra Reflection_Refraction;

subroutine(refl_refra)
vec4 Reflection () {
    // Vertex-Camera vector in world coordinates
    vec3 V = normalize(worldPosition.xyz - cameraPosition);

    // Reflection vector of I w.r.t. normal
    vec3 R = reflect(V, normalize(worldNormal));

    // Sample the texture cube using the components of reflection vector as texture coordinates
    return texture(tCube, R);
}

subroutine(refl_refra)
vec4 Fresnel() {
    vec3 N = normalize(worldNormal);
    vec3 V = normalize(worldPosition.xyz - cameraPosition);

    // Incidence light direction in world coordinates
    vec3 L = normalize(worldPosition.xyz - pointLightPosition);

    // Half vector in world coordinates
    vec3 H = normalize(L + V);

    vec3 R = reflect(V, N);

    vec4 reflectedCol = texture(tCube, R);

    // Refraction vectors:
    //  - one vector for each RGB channel for chromatic aberration
    //  - slightly different Eta, in order to have slightly different colors
    vec3 refractDir[3];
    refractDir[0] = refract(V, N, Eta);
    refractDir[1] = refract(V, N, Eta * 0.99);
    refractDir[2] = refract(V, N, Eta * 0.98);

    // Sample 3 times the cubemap for refraction (1 channel per sample)
    vec4 refractedCol = vec4(1.0);
    refractedCol.r = texture(tCube, refractDir[0]).r;
    refractedCol.g = texture(tCube, refractDir[1]).g;
    refractedCol.b = texture(tCube, refractDir[2]).b;

    float F0 = ((1.0 - Eta) * (1.0 - Eta)) / ((1.0 + Eta) * (1.0 + Eta));
    float ratio = F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0.0), mFresnelPower);

    return mix(refractedCol, reflectedCol, clamp(ratio, 0.0, 1.0));
}

void main(void) {
    colorFrag = Reflection_Refraction();
}