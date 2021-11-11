#version 410 core

out vec4 colorFrag;

in vec3 interp_UVW;

uniform samplerCube tCube;
/*
uniform samplerCube tCube2;
uniform float blendFactor;

uniform vec3 fogColour;

const float lowerLimit = 0.0f;
const float upperLimit = 10.0f;
*/

void main (){
    colorFrag = texture(tCube, interp_UVW);
    /*
    vec4 texture1 = texture(tCube, interp_UVW);
    vec4 texture2 = texture(tCube2, interp_UVW);

    vec4 finalColour = mix(texture1, texture2, blendFactor);
*/
    //colorFrag = finalColour;

    //FOG    
    /*
    vec4 finalColour = texture(tCube, interp_UVW);
    // it represents the visibility of the skybox
    float factor = (interp_UVW.x - lowerLimit) / (upperLimit - lowerLimit);
    // factor is limited between 0 and 1
    factor = clamp(factor, 0.8f, 1.0f);
    colorFrag = mix(vec4(fogColour,1.0f), finalColour, factor);
    */
    
}