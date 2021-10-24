// Based on https://www.youtube.com/watch?v=GZ_1xOm-3qU&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=12&ab_channel=ThinMatrix

#version 410 core

in vec2 textureCoords;
in vec3 surfaceNormal;
// Vector from surface to light source
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec3 u_LightColor;

// Determine how close the camera needs to be to the reflected light to see 
// any change in the brightness
uniform float u_ShineDamper;
// Determine how strong the reflected light should be
uniform float u_Reflectivity;

void main (void) {

    vec2 repeatedTexCoords = mod(textureCoords * 5.0f, 1.0f);
    
    // Ambient light
    vec4 ambient = vec4(u_LightColor, 0.2f) * texture(u_Texture, repeatedTexCoords);

    // Diffuse color
    //  - normalize vectors so that the sizes don't effect the dot product
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    //  - ot product: how bright the pixel should be
    float nDotl = dot(unitNormal, unitLightVector);
    //  - se the min value of brightness value on 0
    float brightness = max(nDotl, 0.0f);
    vec3 diffuse = brightness * u_LightColor;

    // Specular lightnening
    vec3 unitCameraVector = normalize(toCameraVector);
    //  - vector from light source to the object
    vec3 lightDirection = -unitLightVector;
    //  - reflect the light vector
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    //  - dot product: how specular the reflected light should be
    float specularFactor = dot(reflectedLightDirection, unitCameraVector);
    specularFactor = max(specularFactor, 0.0f);
    //  - dampen the brightness w.r.t. the material
    float dampedFactor = pow(specularFactor, u_ShineDamper);
    // The closer u_Reflectivity and dampedFactor are to each other, the brigheter the reflected light is seen
    vec3 finalSpecular = dampedFactor * u_Reflectivity * u_LightColor;

    o_Color = 0.2f * ambient + 0.8f * vec4(diffuse, 1.0f) * texture(u_Texture, repeatedTexCoords) + vec4(finalSpecular, 1.0f);
}