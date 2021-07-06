#version 410 core

#define NR_LIGHTS 3

const float PI = 3.14159265359;

// Output shader variable
out vec4 colorFrag;

// Lights incidence directions
in vec3 lightDirs[NR_LIGHTS];
// Transformed normal calculated in the vertex shader
in vec3 vNormal;
// Vector from fragment to camera
in vec3 vViewPosition;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

// Weight of the components
uniform float Ka;
uniform float Kd;
uniform float Ks;

// Shininess coefficients
uniform float shininess;

// Uniform for GGX model
uniform float alpha;
uniform float F0;

// Type of the subroutine
subroutine vec3 ill_model();
// Subroutine Uniform
subroutine uniform ill_model Illumination_Model_ML;

subroutine(ill_model)
vec3 BlinnPhong_ML() {
    // Ambient component
    vec3 color = Ka * ambientColor;
    // Normalization of the per-fragment normal
    vec3 N = normalize(vNormal);

    for (int i = 0; i < NR_LIGHTS; i++) {
        // Normalization of the per-fragment light incidence direction 
        vec3 L = normalize(lightDirs[i].xyz);

        // Lambert coefficient
        float lambertian = max(dot(L, N), 0.0);

        if (lambertian > 0.0) {
            vec3 V = normalize(vViewPosition);
            vec3 H = normalize(L + V);

            float specAngle = max(dot(H, N), 0.0);
            float specular = pow(specAngle, shininess);

            color += vec3(Kd * lambertian * diffuseColor + 
                          Ks * specular * specularColor);
        }
    }
    return color;
}

float G1 (float angle, float alpha){
    float r = alpha + 1.0;
    float k = (r * r) / 8.0;

    float num = angle;
    float denom = angle *(1.0 - k) + k;

    return num / denom;
}

subroutine(ill_model)
vec3 GGX_ML () {
    vec3 N = normalize(vNormal);
    vec3 lambert = (Kd * diffuseColor) / PI;
    vec3 color = vec3 (0.0);

    for (int i = 0; i < NR_LIGHTS; i++) {
        vec3 L = normalize(lightDirs[i].xyz);
        float NdotL = max(dot(N,L), 0.0);
        vec3 specular = vec3(0.0);

        if (NdotL > 0.0) {
            vec3 V = normalize(vViewPosition);
            vec3 H = normalize(L + V);

            float NdotH = max(dot(N, H), 0.0);
            float NdotV = max(dot(N, V), 0.0);
            float VdotH = max(dot(V, H), 0.0);
            float alpha_squared = alpha * alpha;
            float NdotH_squared = NdotH * NdotH;

            // Geometric factor G2 (Smith's method)
            float G2 = G1(NdotV, alpha) * G1(NdotL, alpha);
            
            // GGX distribution
            float D = alpha_squared;
            float denom = (NdotH_squared * (alpha_squared - 1.0) + 1.0);
            D /= PI * denom * denom;

            // Fresnel reflectance F (Schlink)
            vec3 F = vec3(pow(1.0 - VdotH, 5.0));
            F *= (1.0 - F0);
            F += F0;

            specular = (F * G2 * D) / (4.0 * NdotV * NdotL);

            color += (lambert + specular) * NdotL;
        }
    }
    return color;
}

void main (void) {
    vec3 color = Illumination_Model_ML();
    colorFrag = vec4(color, 1.0);
}