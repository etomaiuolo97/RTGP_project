// Based on https://www.youtube.com/watch?v=GZ_1xOm-3qU&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=12&ab_channel=ThinMatrix

#version 410 core

in vec2 textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec3 u_LightColor;
// It determines how close the camera needs to be to the reflected light to see any change in the brightness
uniform float u_ShineDamper;
uniform float u_Reflectivity;

void main (void) {

    vec2 repeatedTexCoords = mod(textureCoords * 5.0f, 1.0f);
    // Ambient light
    vec4 ambient = vec4(u_LightColor, 0.2f) * texture(u_Texture, repeatedTexCoords);

    // Normalize vectors
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    // How bright the pixel should be
    float nDotl = dot(unitNormal, unitLightVector);
    // If nDotl is less than 0.0f it will set to 0.0f
    float brightness = max(nDotl, 0.0f);
    vec3 diffuse = brightness * u_LightColor;

    vec3 unitCameraVector = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflecredLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor = dot(reflecredLightDirection, unitCameraVector);
    specularFactor = max(specularFactor, 0.0f);
    float dampedFactor = pow(specularFactor, u_ShineDamper);
    // the closer u_Reflectivity and dampedFactor are to each other, the brigheter the reflected light is seen
    vec3 finalSpecular = dampedFactor * u_Reflectivity * u_LightColor;

    o_Color = 0.2f * ambient + 0.8f * vec4(diffuse, 1.0f) * texture(u_Texture, repeatedTexCoords) + vec4(finalSpecular, 1.0f);
}