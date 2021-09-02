#version 410 core

#define NR_LIGHTS 3

// vertex position in world coordinates
layout (location = 0) in vec3 position;
// vertex normal in world coordinate
layout (location = 1) in vec3 normal;
// UV coordinates
layout (location = 2) in vec2 UV;

// vectors of lights positions (passed from the application)
uniform vec3 lights[NR_LIGHTS];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// normals transformation matrix (= transpose of the inverse of the model-view matrix)
uniform mat3 normalMatrix;

// array of light incidence directions (in view coordinate)
out vec3 lightDirs[NR_LIGHTS];

// the transformed normal (in view coordinate) is set as an output variable, to be "passed" to the fragment shader
// this means that the normal values in each vertex will be interpolated on each fragment created during rasterization between two vertices
out vec3 vNormal;

// in the fragment shader, we need to calculate also the reflection vector for each fragment
// to do this, we need to calculate in the vertex shader the view direction (in view coordinates) for each vertex, and to have it interpolated for each fragment by the rasterization stage
out vec3 vViewPosition;

// the output variable for UV coordinates
out vec2 interp_UV;


void main(){
    vec4 mvPosition = viewMatrix * modelMatrix * vec4( position, 1.0 );

    vViewPosition = -mvPosition.xyz;

    vNormal = normalize( normalMatrix * normal );

    for (int i=0; i<NR_LIGHTS; i++) {
        vec4 lightPos = viewMatrix  * vec4(lights[i], 1.0);;
        lightDirs[i] = lightPos.xyz - mvPosition.xyz;
    }

    interp_UV = UV;

    gl_Position = projectionMatrix * mvPosition;
}
